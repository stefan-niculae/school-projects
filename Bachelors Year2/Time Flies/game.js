/* Constants */
var INITIAL_SPEED = 1;
var SPEED_INCREASE = 1.1;
var SCREEN_MIDDLE_BAND = .75;
var MAX_RECENT_SCORES = 3;


/* Document references */
var backgroundImage;

var scoreText;
var playButton;

var splashMenu;
var splashTitle;

var scoreAmounts;
var scoreTimes;

var hardCheckbox;


/* Global variables */
var currentHourglass;
var currentSpeed;

var recentScores = [];


/* Initialization */

// Initialize the game on page load
window.onload = initialize;

function initialize() {
    setupReferences();
    reset();

    centerMenu();
    splashMenu.style.visibility = "visible";
}

function setupReferences() {
    scoreText =         document.getElementById("score");
    playButton =        document.getElementById("play-button");
    backgroundImage =   document.getElementById("bg");
    splashMenu =        document.getElementById("splash-menu");
    splashTitle =       document.getElementById("splash-title");
    scoreAmounts =      document.getElementById("score-amounts");
    scoreTimes =        document.getElementById("score-times");
    hardCheckbox =      document.getElementById("hard-checkbox");
}


/* UI Logic */

function centerHorizontally(object, setCoord) {
    var coord = window.innerWidth / 2 - object.clientWidth / 2;
    object.style.left = coord + "px";

    if (setCoord)
        object.xPos = coord;
}

function increaseScore() {
    scoreText.amount++;
    scoreText.innerHTML = "Score " + scoreText.amount;
}

function clampInTheMiddle(value) {
    var middle = window.innerWidth / 2;
    var bandHalfWidth = (window.innerWidth * SCREEN_MIDDLE_BAND) / 2;

    return value.clamp(middle - bandHalfWidth,
        middle + bandHalfWidth);
}

function centerMenu() {
    var titleSize = 25; // base val
    titleSize += window.innerHeight / window.innerWidth * 50;

    splashTitle.style.width = titleSize.clamp(50, 100) + "%";

    var buttonSize = titleSize / 4;
    playButton.style.width = buttonSize.clamp(15, 30) + "%";

    centerHorizontally(splashTitle);
    centerHorizontally(playButton);
}

window.onresize = function () {
    centerMenu();
};

function scrollToTop() {
    var newScroll = window.scrollY - 25;
    if (newScroll <= 0) {
        newScroll = 0;
        window.scrollTo(0, newScroll);
        clearTimeout(window.scrollTimer);
    }
    else {
        window.scrollTo(0, newScroll);
        window.scrollTimer = setTimeout("scrollToTop()", 1);
    }

}

function updateScores() {
    // Clear old values
    while (scoreAmounts.firstChild)
        scoreAmounts.removeChild(scoreAmounts.firstChild);
    while (scoreTimes.firstChild)
        scoreTimes.removeChild(scoreTimes.firstChild);

    for (var i = 1; i <= MAX_RECENT_SCORES; i++) {
        var index = recentScores.length - i;
        if (index < 0)
            break;

        var createdAmount = document.createElement("li");
        var createdTime = document.createElement("li");

        createdAmount.innerHTML = recentScores[index].amount;
        createdTime.innerHTML = recentScores[index].time;

        scoreAmounts.appendChild(createdAmount);
        scoreTimes.appendChild(createdTime);
    }
}


/* Number Logic */

Number.prototype.clamp = function (min, max) {
    var result = this;
    if (this < min)
        result = min;
    if (this > max)
        result = max;
    return result;
};

function random(min, max, plusMinus) {
    var value = (Math.random() * max) + min;

    if (plusMinus && Math.random() > .5)
        value *= -1;

    return value;
}

Number.prototype.goTowards = function (target, speed) {
    var value = this;

    if (this < target)
        value = this + speed;
    if (this > target)
        value = this - speed;

    return value;
};

function extractScore() {
    var pattern = /\d+/g;
    var score = scoreText.innerHTML.match(pattern);
    if (hardCheckbox.checked)
        score += "!";
    return score;
}


/* Game Control */

function play() {
    splashMenu.style.visibility = "hidden";
    scoreText.style.visibility = "visible";

    if (hardCheckbox.checked)
        currentSpeed = 2 * INITIAL_SPEED;

    createHourglass();
}

function reset() {

    // Resetting the score
    scoreText.innerHTML = "Score 0";
    scoreText.amount = 0;

    scoreText.style.visibility = "hidden";

    // Resetting the fallSpeed
    currentSpeed = INITIAL_SPEED;
}

function restart(manuallyRestarted) {
    var scoreAmount = extractScore();
    reset();
    splashMenu.style.visibility = "visible";
    scrollToTop();

    // No current hourglass on esc press
    if (currentHourglass != null)
        clearHourglass();

    if (!manuallyRestarted) {
        var date = new Date();
        var score = {
            amount: scoreAmount,
            time: date.getHours() + ":" + date.getMinutes()
        };
        recentScores.push(score);
        updateScores();
    }
}


/* Game Logic */

function createHourglass() {
    var created = document.createElement("div");
    document.body.appendChild(created);
    currentHourglass = created;


    // Set the image source and size
    created.setAttribute("id", "hourglass");

    // Make it clickable
    created.setAttribute("onmousedown", "onHourglassClick()");

    // Set the position
    currentHourglass.yPos = window.innerHeight * .13;
    centerHorizontally(created, true);

    // Set the rotation
    currentHourglass.rot = 0;

    // Make it fall and move horizontally every milisecond
    currentHourglass.fallTimer = setInterval(fall, 1);
    currentHourglass.targetX = currentHourglass.xPos;
    currentHourglass.moveHorizTimer = setInterval(moveHorizontally, 1);
}

function clearHourglass() {
    document.body.removeChild(currentHourglass);
    clearInterval(currentHourglass.fallTimer);
    clearInterval(currentHourglass.moveHorizTimer);
}

function fall() {
    currentHourglass.yPos += currentSpeed;
    currentHourglass.style.top = currentHourglass.yPos + "px";

    if (hasReachedBottom())
        restart();
}

function moveHorizontally() {

    // If we reached the target (the quanta is _currentSpeed_
    if (Math.abs(currentHourglass.targetX - currentHourglass.xPos) <= currentSpeed)
        pickNewTargetX();

    else {
        currentHourglass.xPos = currentHourglass.xPos.goTowards(
            currentHourglass.targetX,
            currentSpeed);
        currentHourglass.style.left = currentHourglass.xPos + "px";

        rotateTowardsTarget();
    }
}

function rotateTowardsTarget() {
    if (currentHourglass.xPos < currentHourglass.targetX)
        currentHourglass.rot += currentSpeed;
    if (currentHourglass.xPos > currentHourglass.targetX)
        currentHourglass.rot -= currentSpeed;

    currentHourglass.style.transform = "rotate(" + currentHourglass.rot + "deg)";
}

function pickNewTargetX() {
    var value = random(.06, .08, true) * window.innerWidth;
    value += window.innerWidth / 2;
    currentHourglass.targetX = clampInTheMiddle(value);
}

function hasReachedBottom() {
    var bottomOfHourglass = currentHourglass.yPos + currentHourglass.clientHeight;
    return bottomOfHourglass >= backgroundImage.clientHeight;
}

function onHourglassClick() {
    // Increase the speed
    currentSpeed *= SPEED_INCREASE;
    increaseScore();

    clearHourglass();
    createHourglass();

    scrollToTop();
}


/* Input Listening */
document.onkeydown = function (event) {

    event = event || window.event;

    // On enter press
    if (event.keyCode == 13) {
        console.log(playButton.style.visibility);
        // If the play button is clickable
        if (splashMenu.style.visibility == "visible")
            play();
    }

    // On escape press
    if (event.keyCode == 27) {
        // By default, esc stops the page from loading
        event.preventDefault();
        console.log("pressed esc");
        restart(true);
    }
};

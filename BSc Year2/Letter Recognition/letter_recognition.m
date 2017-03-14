[alphabet, targets] = prprob;

S1 = 10;
[R,~] = size(alphabet);
[S2,Q] = size(targets);

P = alphabet;
T = targets;

net = newff(minmax(P),[S1 S2],{'logsig' 'logsig'},'traingdx'); 
net.LW{2,1} = net.LW{2,1}*0.01;
net.b{2} = net.b{2}*0.01;

% Training with no noise
net.trainParam.epochs = 5000;
net.trainParam.goal = 0.05;
net.performFcn = 'sse';
net.trainParam.mc = 0.95;

[net, tr] = train(net, P, T);


% Training with noise
for i = 1:10
    P = [alphabet alphabet ...
         alphabet + 0.1 * randn(size(alphabet)) ...
         alphabet + 0.2 * randn(size(alphabet))];
    T = [targets targets targets targets];
    net = train(net, P, T);
end


misclassified = 0;
figure('name', 'input image next to predicted letter')

for i = 1:26
    
    bruiata = alphabet(:,i) + randn(35, 1) * .3;
    
    output = sim(net, bruiata);
    output = compet(output);
    
    % noisy letter
    subplot(5, 12, 2 * i - 1)
    plotchar(bruiata);
    
    % guessed letter
    subplot(5, 12, 2 * i)
    plotchar(alphabet(:, find(output)));

    if find(output) ~= i
        misclassified = misclassified + 1;
        asciiCode = 64 + i;
        setstr(asciiCode)
    end
    
end

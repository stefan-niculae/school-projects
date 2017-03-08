figure('name', '3D Loss')

% Training points and labels
P = [ -2 -2 -1 -1  0  0  0  0  1  1  2  2  3  3  3  3  4  4  5  5 ; ...
       2  3  1  4  0  1  2  3  0  1  1  2 -1  0  1  2 -2  1 -1  0 ];
l = [ -1 -1 -1 -1 -1 -1 -1 -1  1 -1  1 -1  1  1  1  1  1  1  1  1 ];
S = [ P; l ];

% m - number of points in training set
[~, m] = size(P);



A = -1 : .5 : 3;
B = -2 : .25 : 1;

[~, p] = size(A);
[~, q] = size(B);

[X, Y] = meshgrid (A, B);



net = newlin (P, l);

J = zeros(size(X));
for i = 1:p * q
    net.IW{1, 1} = [X(i) Y(i)];
    h = sim(net, P);
    J(i) = sse(l - h);
end

surf(X, Y, J);
hold on


net.trainParam.epochs = 1;
w = [0 0];
net.IW{1, 1} = [2.5 .75];

for i = 1:50
    w = net.IW{1, 1};
    net.b{1} = 0;
    h = sim(net, P);
    err = sse(l - h);
    
    plot3(w(1), w(2), err, 'ow');
    
    net = train(net, P, l);
end

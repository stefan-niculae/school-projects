function [ w, s ] = step_perceptron( x, l, lr, f )
% x - training set (no bias)
% l - labels
% lr - learning rate
% f - transfer function

    origX = x;

    % N - dimension of a point
    [N, ~] = size(x);

    % Add 1 to each x for bias
    x (N + 1, :) = 1;
    
    % Initialize weights
    w = rand(N + 1, 1);  % N+1 for bias (last component)
    
    figure('name', 'Step Perceptron');
  
    maxEpochs = 100;
    for epoch = 1:maxEpochs
        % Compute label
        n = x' * w;
        y = f(n)';
        
        % Update weights in case of errors
        w = w + (lr * (l - y) * x')'; 
        
        % Wait a bit (to create the animation effect)
        pause (.05);
        
        % Plot points
        plotpv (origX, l);
        hold  off;
        
        % Plot line
        hw = w';
        line = plotpc (hw(1:2), hw(3));
        set(line, 'Color', 'r');
        
        % Stop if all points classified correctly
        if isequal(l, y)
            s = epoch;
            break;
        end
    end
   
end

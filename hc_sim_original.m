function [ hc ] = hc_sim(A,b_percent,step_t,lazy_value,initial_quantity)

%========================================================

% A is the adjacency matrix of the graph.

% b_percent is the percent of boundary nodes, in Shan's experiment she use
% 0.04. 

% step_t is the number of steps for the random walk simulation

% lazy_value is the probability that a random walker move out ( <= 1 ) in
% lazy random walk situation. Small lazy_value will lead to smoother heat
% content curve. 

% initial_quantity is how much random walkers are initialed in the
% simulation on one node, more random walkers means more precise the result
% is but also more computation time.

% hc is the unnormalized simulation result of heat content

%===================================================
% Calculate the random walk matrix
sA = size(A,1);
P = zeros(sA,sA);
for i = 1:sA
    P(i,:) = A(i,:) / sum(A(i,:));
end

Degree = sum(A,1);

% select the boundary nodes of the undirected graph
sn = size(Degree,2);
sb = b_percent * sn;
B = zeros(1,sb);

degtemp = max(Degree) + 10;

for i = 1:sb
    mintemp = find(Degree == min(Degree));
    rdi = randi(size(mintemp,2));
    B(1,i) = mintemp(1,rdi);
    Degree(1,mintemp(1,rdi)) = degtemp;
end

% initialization of the random walk
step=step_t*1 / lazy_value;

ini_dist = ones(1,sA);
if B(1,1) ~= 0
    for i = 1:sb
        ib = B(i);
        ini_dist(ib) = 0;
    end
end

ini_dist = ini_dist .* initial_quantity;

hc = zeros(step + 1,1);
hc(1) = sum(ini_dist);

% rnd_wal is a matrix contains all the information of the random walkers in 
% simulation, rnd_wal(x,1) is a label, rnd_wal(x,2) is the initial node of random walker x,
% rnd_wal(x,3) is the current node of random walker x

rnd_wal = zeros(sum(ini_dist),3);

x = 1;
for i = 1:sA
    if ini_dist(i) ~= 0
        for j = 1:ini_dist(i)
            rnd_wal(x,1) = 1;
            rnd_wal(x,2) = i;
            rnd_wal(x,3) = i;
            x = x + 1;
        end
    end
end

rnd_wal_size = size(rnd_wal,1);

for i = 1:step
    
    next_hc = hc(i);
    
    for j = 1:rnd_wal_size
        
        flag = rnd_wal(j,1); 
        
        if(flag == 1)
            if (rand < lazy_value)
                
                begin_node = rnd_wal(j,3);
                m = rnd_wal(j,2);
                n = rnd_wal(j,3);
                
                delta_minus = 1 * sqrt(Degree(m) / Degree(n));
                next_hc = next_hc - delta_minus;
                
                temp = rand;
                sumtemp = P(begin_node,1);
                x = 1;
                
                while(temp > sumtemp)
                    x = x + 1;
                    sumtemp = sumtemp + P(begin_node,x);
                end
                
                if all(B-x) == 1 
                    delta_plus = 1 * sqrt(Degree(m) / Degree(x));
                    next_hc = next_hc + delta_plus;
                    rnd_wal(j,3) = x;
                else
                    rnd_wal(j,1) = 0;
                end
            end
        end
        
    hc(i+1) = next_hc;
    end
    
end



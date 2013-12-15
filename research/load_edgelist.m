function [ A ] = load_edgelist( filename, n )
%load_edgelist loads a comma-seperated from,to edgelist with n nodes
%              returns an adjacency matrix

edges = load(filename);
edges = edges + 1;  %nodes are 0-indexed, matlab is 1-indexed
%A = sparse(zeros(n,n));
A = zeros(n,n); 
for i = 1:length(edges)
    A(edges(i,1), edges(i,2)) = 1;
end

end


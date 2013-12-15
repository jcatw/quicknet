function [] = similarityexperiment( linear_name, variable_fitness_name, nonlinear_name, outfile, n )

%debug_on_warning(1);
	 
b_percent = 0.04;
step_t = 20;
lazy_value = 0.5;
initial_quantity = 10;

fprintf(1,"Loading Edges.\n");
A_linear = load_edgelist(linear_name,n);
A_variable_fitness = load_edgelist(variable_fitness_name,n);
A_nonlinear = load_edgelist(nonlinear_name, n);

fprintf(1,"Simulating.\n")
fprintf(1,"Linear.\n")
heat_linear = hc_sim(A_linear,b_percent,step_t,lazy_value,initial_quantity);
fprintf(1,"Variable.\n")
heat_variable_fitness = hc_sim(A_variable_fitness,b_percent,step_t,lazy_value,initial_quantity);
fprintf(1,"Nonlinear.\n")
heat_nonlinear = hc_sim(A_nonlinear,b_percent,step_t,lazy_value,initial_quantity);

fid = fopen(outfile,'w');
for i = 1:(step_t + 1)
    fprintf(fid,
	    "%f,%f,%f\n",
	    heat_linear(i),
	    heat_variable_fitness(i),
	    heat_nonlinear(i));
end
fclose(fid)
end


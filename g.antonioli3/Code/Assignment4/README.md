#Assignment 4
I need to create a thread pool able to get and execute functions. So once created i have to get the pids and store them 
until i have some work for them. I have to imagine having dead periods with no functions executed and periods where the
fucntions will be more than the actual threads. This means i need a protected queue. My main will then loop on getting
a task and executing it.
How do i get the task? Most classical/convenient way-> have smth in the queue to call. we can use std::bind that takes 
function and arg ```auto fx=std::bind(f,args)```.


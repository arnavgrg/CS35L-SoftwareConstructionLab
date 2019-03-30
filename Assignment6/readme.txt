/////////
README
/////////

After reading through the code, I realized very quickly that
the main was working for one string. To make it multithreaded,
I would have to allow for more than one string. So, I deleted
the section of the code that checked if the value of nthreads was not equal to 1. 

I then noticed that there was a section within the main function
that was visiting each pixel. I decided to take this code
segment and create a separate function called multithreadFunc
which would be called by the pthread_create method within the 
main. This function takes one argument. 

Another thing that I didn't initially figure out was that I had 
to create memory for the thread IDs to pass them into the 
function, but the pthread_create method takes a void* argument. 
If I didn't do this, the pixel colors calculated by the threads
seemed to lack accuracy.

I moved the print statements back to the main function. This is 
because printing them without the threads joining together
affected the output and layout of pixels on my first attempt.
The only way to avoid this was to create a separate array to 
store the data of the pixels and then output that data 
separately within the main function. 

My make file also warned of a clock skew warning.
"Clock skew detected. Your build may be incomplete".
Although the build was complete, I believe this message was 
output to the terminal because there was difference between
my machine's clock time and the server's time.

The performance overall increases tremendously. I can conclude 
this based on the time values that are printed out on the 
terminal:

			time ./srt 1-test.ppm >1-test.ppm.tmp
			real 0m42.377s
			user 0m42.370s
			sys  0m0.001s

			time ./srt 2-test.ppm >2-test.ppm.tmp
			real 0m22.410s
			user 0m43.400s
			sys  0m0.000s

			time ./srt 4-test.ppm >4-test.ppm.tmp
			real 0m14.755s
			user 0m44.060s
			sys  0m0.002s

			time ./srt 8-test.ppm >8-test.ppm.tmp
			real 0m9.261s
			user 0m44.115s
			sys  0m0.001s

Clearly, there is a direct correlation between the number of threads and the time taken to complete the program. As the 
number of threads increases, the overall time taken to complete
the ray-tracer program decreases. Infact, using 8 threads
instead of 1 stead almost speeds up the process by 5 times. 

Out of curiosity, I also decided to test 16, 32 and 64 threads 
on the ray-tracer program. The value seemed to approach a 
constant value of 5s. 

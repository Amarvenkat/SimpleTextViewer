# SimpleTextViewer
A Text Viewer that reads the callgrind files and compute the total cost for every function by summing its own cost and function cost.
The Text Viewer highlight the function, its own cost and call cost in gradient of Red color. i.e Deep Red for higher cost lighter Red for less cost.
It also highlight the important Header Lines in blue.
It also provides a function name and its cost at the bottom. The function with hightest cost appear at the top followed by functions with lower costs.

Ref: "https://valgrind.org/docs/manual/cl-format.html". 

Can also highlight other texts when searched. 

# Inspecting Linux Kernel State

## Description of logic:
The program tells the user about various states of the emulated Linux Kernel State. The user can specify whether they want the short format which only gives a little information, or the long format which gives more information on the kernel state. To accomplish this I created a function for each aspect of the project and in the main method had a switch case to check the user input. Once I found the location of all the important files, I just used simple string parsing to obtain the correct info.
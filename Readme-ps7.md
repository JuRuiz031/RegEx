# PS7: Kronos Log Parsing

## Contact
Name: Juan Ruiz
Section: 204
Time to Complete: ~10 hours (issues with correct regex and with output)


## Description
Parses through a log file for the Kronos InTouch device and generates reports
on start and end times and their relevant successes or failures

### Features
Decided to make a print function to print the result. This helped with readability
for me as well as with knowing what the formatting was supposed to be by comparing to
the given zip files.

### Approach
I started by first looking through regex documentation, then I checked what the desired
output needed to look like for formatting. From there I worked my way back by looking
at the sample input and the sample output to see what order I needed for the given
regex expressions

### Regex
The startRegex and endRegex are regular expressions used to parse specific log entries. 
startRegex captures lines marking the startup, identified by a straightforward timestamp
and a log message confirming the device's activation. Conversely, endRegex targets 
entries signaling the activation of components, detailed with a timestamp
including milliseconds and more specific details.

### Issues
Just getting 

### Extra Credit

## Acknowledgements
Tutoring center as always! Other than that just a lot of looking up documentation and
websites for how to use regex and what the right ones for the formatting should be

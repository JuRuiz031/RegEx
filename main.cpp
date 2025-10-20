//  Copyright Juan Fernando Ruiz 2024
#include <iostream>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

// STDIO Using statements
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::pair;
using std::cerr;
using std::locale;

// Boost library regex and time Using statemens
using boost::regex;
using boost::smatch;
using boost::posix_time::ptime;
using boost::posix_time::time_duration;
using boost::posix_time::time_facet;
using boost::gregorian::date;
using boost::gregorian::from_simple_string;
using boost::gregorian::date_period;
using boost::gregorian::date_duration;


void printResult(ofstream& os,
  const pair<ptime, int>& startTime,
  const pair<ptime, int>& endTime,
  bool serverStarted,
  int bootTimeMs,
  const string& filename,
  int lineNumber);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Command line error" << endl;
        exit(1);
    }
    string time;
    string file(argv[1]);
    string outputfile = file + ".rpt";
    pair<ptime, int> dateStart(ptime(), 0);
    pair<ptime, int> dateEnd(ptime(), 0);

    ofstream output(outputfile);
    if (!output.is_open()) {
        cout << "Output failed to open." << endl;
        exit(1);
    }

    ifstream inputFile(file);
    if (!inputFile.is_open()) {
        cout << "Failed to open " << file << endl;
        exit(1);
    }

// Start of log message for timestamp and process info
    regex reg;
    regex startRegex("(\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}): "
        "\\(log\\.c\\.166\\) server started");
    regex endRegex("(\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}\\.\\d{3}):"
        "INFO:oejs\\.AbstractConnector:Started "
        "SelectChannelConnector@\\d+\\.\\d+\\.\\d+\\.\\d+:\\d+");

    // Read each line of input and process potential start/end messages
    string line;
    int lineCount = 0;
    while (getline(inputFile, line)) {
        lineCount++;
        smatch match;

        // Match "start" messages, extract timestamp, and handle output
        if (regex_search(line, match, startRegex)) {
            date logDate(from_simple_string(line.substr(0, 10)));
            ptime startTime(logDate, time_duration(stoi(line.substr(11, 13)),
            stoi(line.substr(14, 16)),
            stoi(line.substr(17, 19))));

            // Check if a previous start time exists and print result
            if (dateStart.second != 0) {
                printResult(output, dateStart, dateEnd, false,
                0, file, lineCount);
            }
            dateStart = {startTime, lineCount};

        //  Match "end" messages
        } else if (regex_search(line, match, endRegex)) {
            date logDate(from_simple_string(line.substr(0, 10)));
            ptime endTime(logDate, time_duration(stoi(line.substr(11, 13)),
            stoi(line.substr(14, 16)),
            stoi(line.substr(17, 19))));

            // Assign end time and calculate duration
            dateEnd = {endTime, lineCount};
            time_duration duration = dateEnd.first - dateStart.first;

            // Print results and reset start/end times
            printResult(output, dateStart, dateEnd, true,
            duration.total_milliseconds(), file, lineCount);
            dateStart = {dateStart.first, 0};
            dateEnd = {dateEnd.first, 0};
        }
    }

    inputFile.close();
    output.close();

    return 0;
}

void printResult(ofstream& os,
                 const pair<ptime, int>& startTime,
                 const pair<ptime, int>& endTime,
                 bool serverStarted,
                 int bootTimeMs,
                 const string& filename,
                 int lineNumber) {
  // Set time format for output
  time_facet* facet = new time_facet("%Y-%m-%d %H:%M:%S");
  facet->month_format("%m");
  os.imbue(locale(os.getloc(), facet));

  // Print device boot message
  os << "=== Device boot ===" << endl;

  // Print boot start time and its specific line number
  os << startTime.second << "(" << filename << "): "
     << startTime.first << " Boot Start\n";

  if (serverStarted) {
    // Print boot completion time and its specific line number
    os << endTime.second << "(" << filename << "): "
       << endTime.first << " Boot Completed\n";
    os << "\tBoot Time: " << bootTimeMs << "ms" << endl;
  } else {
    // Print incomplete boot message
    os << "**** Incomplete boot ****" << endl;
  }

  os << endl;
}

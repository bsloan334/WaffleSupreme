#pragma once
#include <cstdlib>
#include <cstdint>
#include <iostream>

using namespace std;

const int NEW_STATE = 1;
const int READY_STATE = 2;
const int WAITING_STATE = 3;
const int RUNNING_STATE = 4;
const int TERMINATED_STATE = 5;

const int FIRST_COME_FIRST_SERVE = 1;
const int ROUND_ROBIN = 2;
const int SHORTEST_JOB_FIRST = 3;

const int MEM_NULL = -1;

// Constant keys for scanning file
const string JOB_START = "// JOB";
const string DATA_START = "// DATA";
const string JOB_END = "// END";

// Maximum length of file line
const size_t MAX_LINE_LEN = 17;
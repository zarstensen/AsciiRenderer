#pragma once

#include <functional>
#include <memory>
#include <algorithm>
#include <math.h>
#include <thread>
#include <chrono>
#include <filesystem>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <future>
#include <vector>
#include <array>
#include <tuple>
#include <assert.h>

#ifdef AR_WIN
#include <Windows.h>
#else
#include <sys/ioctl>
#include <unistd>
#endif

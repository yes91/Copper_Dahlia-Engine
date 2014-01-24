#pragma once

//Standard Header Files
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>
#include <regex>
#include <functional>
#include <memory>

using std::shared_ptr;
using std::weak_ptr;
using std::static_pointer_cast;
using std::dynamic_pointer_cast;
using std::unique_ptr;

#define SAFE_DELETE(p) { if (p) { delete (p); (p)=nullptr; } };


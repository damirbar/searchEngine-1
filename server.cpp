#include <iostream>
#include <thread>
#include <unistd.h>
#include <cstdio>
#include <string>
#include "server.hpp"


int main(int argc, char const *argv[]) {
  Server s;
  s.search("eran");
  return 0;
}

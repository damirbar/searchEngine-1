#pragma once
//#include <mutex>
#include <string>
#include <thread>
#include <iostream>
#include <fstream>
#include "trie.cpp"

using namespace std;


class Server
{
public:
  size_t poolSize;
  trie t;
  Server()
  {
    t->root=new node;
    poolSize=0;
  }
  void search(String str)
  {
    t.go(READ,str);
  }
}

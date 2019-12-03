#ifndef _REGALLOC_
#define _REGALLOC_

#include <iostream>
#include "graph.h"
#include "graphColoring.h"
#include "liveness.h"
#include "AST.h"

void aliveVariables(Program *p);

#endif
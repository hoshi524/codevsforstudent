#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import subprocess
import sys
import random

W = 10
H = 16
T = 3
HT = H + T
S = 10
N = 500
OBS = S + 1

def randomPack():
    pack = [[0 for i in range(T)] for j in range(T)]
    for i in range(3):
        pack[random.randrange(0,T)][random.randrange(0,T)] = random.randrange(1,S)
    return pack

def rot1(pack):
  rotated = [ [ 0 for i in range(T) ] for j in range(T) ]
  for i in range(T):
    for j in range(T):
      rotated[j][T - 1 - i] = pack[i][j]
  return rotated

class game:

    def __init__(self):
        self.packs = [randomPack() for i in range(N)]
        self.obs = [0 for i in range(2)]

    def initInput(self):
        input = '%d %d %d %d %d\n' %(W,H,T,S,N)
        for pack in self.packs:
            for i in range(T):
                input += ' '.join(map(str,pack[i])) + '\n'
            input += 'end\n'
        return input

if __name__ == '__main__':
    battle = sys.argv[1]
    aiCommand0 = sys.argv[2]
    aiCommand1 = sys.argv[3]
    print(battle,aiCommand0,aiCommand1)
    game = game()

    ai0 = subprocess.Popen(aiCommand0,stdin = subprocess.PIPE,stdout = subprocess.PIPE,stderr = subprocess.PIPE)
    print(ai0.stdout.readline())
    ai0.stdin.write(bytes(game.initInput(), "ascii"))
    ai0.stdin.flush()
    
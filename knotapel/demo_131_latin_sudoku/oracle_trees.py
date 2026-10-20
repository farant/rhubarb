#!/usr/bin/env python3
"""D131 oracle for the tree table: projective and mild tree counts on
n <= 5 words (one root word, artificial root 0). Independent of main.c
and lib/dependentia: yields by recursion, projective = every yield an
interval, mild = gap degree <= 1 and no two disjoint yields interleave.
Prints n, trees, projective, mild. Usage: python3 -I oracle_trees.py
"""
import itertools
def trees(n):
    for hs in itertools.product(range(n+1), repeat=n):
        if any(hs[k]==k+1 for k in range(n)): continue
        if sum(1 for h in hs if h==0)!=1: continue
        ok=True
        for k in range(n):
            j=k; s=0
            while hs[j]!=0:
                j=hs[j]-1; s+=1
                if s>n: ok=False; break
            if not ok: break
        if ok: yield hs
def yields(hs,n):
    ch={k:[] for k in range(1,n+1)}
    for d,h in enumerate(hs,1):
        if h: ch[h].append(d)
    Y={}
    def y(k):
        if k in Y: return Y[k]
        s={k}
        for c in ch[k]: s|=y(c)
        Y[k]=s; return s
    for k in range(1,n+1): y(k)
    return Y
def gaps(s):
    t=sorted(s); return sum(1 for a,b in zip(t,t[1:]) if b-a>1)
def dominates(Y,a,b): return b in Y[a]
for n in range(1,6):
    proj=mild=tot=0
    for hs in trees(n):
        tot+=1; Y=yields(hs,n)
        gd=max(gaps(Y[k]) for k in Y)
        wn=True
        for a,b in itertools.combinations(range(1,n+1),2):
            if dominates(Y,a,b) or dominates(Y,b,a): continue
            A,B=Y[a],Y[b]
            if any(l1<l2<r1<r2 for l1 in A for r1 in A for l2 in B for r2 in B) or \
               any(l2<l1<r2<r1 for l1 in A for r1 in A for l2 in B for r2 in B): wn=False; break
        if gd==0: proj+=1
        if gd<=1 and wn: mild+=1
    print(n,tot,proj,mild)

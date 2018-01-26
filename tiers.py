""" Tier list maker
	Input: A file containing a matrix of matchups
		every character name must be a single character
		every character matchup should be expressed as an integer (0-100)
	Output: different types of tier lists
"""
import sys
import math

# returns a list of scores of the characters
def total_list(mat):
	score_lst = []
	for char in mat:
		tot = 0
		for mu in char:
			tot += mu
		score_lst.append(tot)
	return score_lst

# N rep with Manhattan weight normalization
def n_rep_list(mat, n):
	score_lst = []
	weight = [1] * len(mat)
	for i in range(0,n):
		n_weights = []
		for char in mat:
			tot = 0
			for i in range(0,len(char)):
				tot += char[i] * weight[i]
			n_weights.append(tot)
		weight = [float(i)/sum(n_weights) for i in n_weights]
	return n_weights


# First implementation (Manhattan Normalization)
# finds an equilibrium solution to the tier list problem
# not an eigenvalue of the matrix, but inspired by eigenvalues
def iterlist(mat):
	n = 1
	prev = [0] * len(mat)
	curr = [1] * len(mat)
	dist = 1
	while dist > 0.05:	# stop at some arbitrary tolerance
		n_weights = []
		for char in mat:
			tot = 0
			for i in range(0,len(char)):
				tot += char[i] * curr[i]
			n_weights.append(tot)
		curr = [float(i)/sum(n_weights) for i in n_weights]
		dist = abs(sum([prev[i] - curr[i] for i in range(len(prev))]))
		prev = curr
		n = n + 1
		if n > 500 * len(mat): # check that you don't infinite loop
			return [0]
	return n_weights


# Second Implementation (Euclidean Norm)
# Similar to the above iterative tier list, but normalizes the weights
# with respect the to the euclidean distancing function
def iterlist2(mat):
	n = 1
	prev = [0] * len(mat)
	curr = [1] * len(mat)
	dist = 1
	while dist > 0.05:	# stop at some arbitrary tolerance
		n_weights = []
		for char in mat:
			tot = 0
			for i in range(0,len(char)):
				tot += char[i] * curr[i]
			n_weights.append(tot)
		norm = math.sqrt(sum([i**2 for i in n_weights]))
		curr = [float(i)/norm for i in n_weights]
		dist = abs(sum([prev[i] - curr[i] for i in range(len(prev))]))
		prev = curr
		n = n + 1
		if n > 500 * len(mat): # check that you don't infinite loop
			return [0]
	return n_weights


def collection(filename):
	names = []
	mat = []
	f = file(filename, 'r')
	for line in f:
		if line[0] == '#':
			continue
		spread = []
		elements = line.replace('\t', ' ').split(' ')
		names.append(elements.pop(0))
		for e in elements:
			if e != '':
				spread.append(int(e))
		mat.append(spread)
	return names, mat


def printlist(names, scores):
	pairing = zip(names, scores)
	pos = 1
	cnt = 1
	last_score = 0
	for key, value in sorted(pairing, key=lambda (k,v): (v,k), reverse=True):
		if value != last_score:
			pos = cnt
		if last_score == 0:
			print "%s: %s  \t(%.3f)" % (pos, key, value)
		else:
			print "%s: %s  \t(%.3f) [%.3f]" % (pos, key, value, value-last_score)
		cnt = cnt + 1
		last_score = value


def main():
	if len(sys.argv) <= 1:
		print "Specify a matrix file please"
		return
	names, mat = collection(sys.argv[1])
	scores = iterlist2(mat)
	printlist(names, scores)



if __name__ == '__main__':
	main()

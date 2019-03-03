from Bio import pairwise2
from Bio.pairwise2 import format_alignment
from time import process_time

if __name__ == "__main__":
    seq1File = open("seq1.txt", "r")
    seq2File = open("seq2.txt", "r")

    seq1 = seq1File.read(100)
    seq2 = seq2File.read(100)

    scoreDict = {}

    for c1 in ['A', 'C', 'T', 'G']:
        for c2 in ['A', 'C', 'T', 'G']:
            if (c1 == c2):
                scoreDict[(c1, c2)] = 2
            else:
                scoreDict[(c1, c2)] = -1

    seq1File.close()
    seq2File.close()

    start = process_time()

    alignments = pairwise2.align.globalms(seq1, seq2, 2, -1, -2, -2, score_only=True)

    print(alignments)

    end = process_time() - start

    print(end)

    alignments = pairwise2.align.localms(seq1, seq2, 2, -1, -2, -2, score_only=True)

    print(alignments)

    end = process_time() - end

    print(end)
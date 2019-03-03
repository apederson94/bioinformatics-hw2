#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

struct swRetVal
{
    int score;
    int *scoreMatrix;
    char *traceMatrix;
};

int max(int val1, int val2)
{
    if (val1 > val2)
    {
        return val1;
    }

    return val2;
}

int getPairValue(char c1, char c2, int *subMat)
{
    int pos1, pos2;

    switch(c1)
    {
        case 'A':
            pos1 = 0;
            break;
        case 'C':
            pos1 = 1;
            break;
        case 'G':
            pos1 = 2;
            break;
        case 'T':
            pos1 = 3;
            break;
        default:
            return -1;
    }

    switch(c2)
    {
        case 'A':
            pos2 = 0;
            break;
        case 'C':
            pos2 = 1;
            break;
        case 'G':
            pos2 = 2;
            break;
        case 'T':
            pos2 = 3;
            break;
        default:
            return -1;
    }

    return subMat[pos1 + (pos2 * 4)];
}

struct swRetVal* smithWaterman(int *subMat, int gapPenalty, char *seq1, char *seq2)
{
    int seq1Pos, seq2Pos, diagUp, diagDown, up, left, maximum, score;
    int seq1Len = strlen(seq1) + 1;
    int seq2Len = strlen(seq2) + 1;
    int matLen = seq1Len * seq2Len;
    int *scoreMatrix = (int *) calloc(matLen, sizeof(int));
    struct swRetVal *ret = (struct swRetVal*) calloc(1, sizeof(struct swRetVal));
    char *traceMatrix = (char *) calloc(matLen, sizeof(char));

    score = 0;

    //initializing score matrix
    for (seq1Pos = 0; seq1Pos < seq1Len; seq1Pos++)
    {
        scoreMatrix[seq1Pos] = 0;
        traceMatrix[seq1Pos] = 'x';
    }

    for (seq2Pos = 0; seq2Pos < seq2Len; seq2Pos++)
    {
        scoreMatrix[seq2Pos * seq1Len] = 0;
        traceMatrix[seq2Pos * seq1Len] = 'x';
    }

    for (seq2Pos = 1; seq2Pos < seq2Len; seq2Pos++)
    {
        for (seq1Pos = 1; seq1Pos < seq1Len; seq1Pos++)
        {
            diagUp = scoreMatrix[(seq1Pos - 1) + ((seq2Pos - 1) * seq1Len)] + getPairValue(seq1[seq1Pos - 1], seq2[seq2Pos - 1], subMat);

            diagDown = 0; 
            
            left = scoreMatrix[(seq1Pos - 1) + (seq2Pos * seq1Len)] + gapPenalty;

            up = scoreMatrix[seq1Pos + ((seq2Pos - 1) * seq1Len)] + gapPenalty;

            maximum = max(max(max(diagUp, diagDown), left), up);

            scoreMatrix[seq1Pos + (seq2Pos * seq1Len)] = maximum;

            if (diagUp == maximum)
            {
                traceMatrix[seq1Pos + (seq2Pos * seq1Len)] = '\\';
            }
            else if (diagDown == maximum)
            {
                //DIAGONAL UP LOOKS THE SAME AS DIAGONAL DOWN SO I USED '>' INSTEAD
                traceMatrix[seq1Pos + (seq2Pos * seq1Len)] = '>';
            }
            else if (up == maximum)
            {
                traceMatrix[seq1Pos + (seq2Pos * seq1Len)] = '^';
            }
            else if (left == maximum)
            {
                traceMatrix[seq1Pos + (seq2Pos * seq1Len)] = '<';
            }

            if (score < maximum)
            {
                score = maximum;
            }
        }
    }

    ret->score = score;
    ret->scoreMatrix = scoreMatrix;
    ret->traceMatrix = traceMatrix;

    return ret;
}

int main(int argc, char const *argv[])
{
    struct timeval start, end;
    double secDiff, usecDiff;
    int subMat[16] = {
        1, 0, 0, 0, 
        0, 1, 0, 0, 
        0, 0, 1, 0, 
        0, 0, 0, 1
        };

    FILE *seq1File = fopen("seq1.txt", "r");
    FILE *seq2File = fopen("seq2.txt", "r");

    char *seq1, *seq2;
    seq1 = (char *) calloc(1000001, sizeof(char));
    seq2 = (char *) calloc(251, sizeof(char));
    fgets(seq1, 1000001, seq1File);
    fgets(seq2, 251, seq2File);

    gettimeofday(&start, NULL);
    struct swRetVal *new = smithWaterman(subMat, 0, seq1, seq2);
    gettimeofday(&end, NULL);

    secDiff = end.tv_sec - start.tv_sec;
    usecDiff = end.tv_usec - start.tv_usec;

    if (usecDiff < 0)
    {
        usecDiff += 1000000;
        secDiff--;
    }

    printf("Score: %d\n", new->score);
    printf("Time: %lf\n", secDiff + (usecDiff/1000000));

    free(new->traceMatrix);
    free(new->scoreMatrix);
    free(new);

    return 0;
}
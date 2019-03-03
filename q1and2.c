#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

struct retVal
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
            exit(1);
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
            exit(1);
    }

    return subMat[pos1 + (pos2 * 4)];
}

struct retVal* needlemanWunsch(int *subMat, int gapPenalty, char *seq1, char *seq2)
{
    int seq1Pos, seq2Pos, diag, up, left, maximum, score, seq1CharPos, seq2CharPos, currentPos;
    int seq1Len = strlen(seq1) + 1;
    int seq2Len = strlen(seq2) + 1;
    int matLen = seq1Len * seq2Len;
    int *scoreMatrix = (int *) calloc(matLen, sizeof(int));
    struct retVal *ret = (struct retVal*) calloc(1, sizeof(struct retVal));
    char *traceMatrix = (char *) calloc(matLen, sizeof(char));

    //initializing score & trace matrices
    for (seq1Pos = 0; seq1Pos < seq1Len; seq1Pos++)
    {
        scoreMatrix[seq1Pos] = -seq1Pos;
        scoreMatrix[seq1Pos * seq1Len] = -seq1Pos;
        traceMatrix[seq1Pos] = 'x';
        traceMatrix[seq1Pos * seq1Len] = 'x';
    }

    //iterating over matrix and string comparing characters/scores
    for (seq2Pos = 1; seq2Pos < seq2Len; seq2Pos++)
    {
        for (seq1Pos = 1; seq1Pos < seq1Len; seq1Pos++)
        {
            currentPos = seq1Pos + (seq2Pos * seq1Len);

            diag = scoreMatrix[currentPos - (seq1Len + 1)] + getPairValue(seq1[seq1Pos - 1], seq2[seq2Pos - 1], subMat); 
            
            left = scoreMatrix[currentPos - 1] + gapPenalty;

            up = scoreMatrix[currentPos - seq1Len] + gapPenalty;

            scoreMatrix[currentPos] = max(max(diag, left), up);

            maximum = max(max(diag, left), up);

            if (diag == maximum)
            {
                traceMatrix[currentPos] = '\\';
            }
            else if (left == maximum)
            {
                traceMatrix[currentPos] = '<';
            }
            else if (up == maximum)
            {
                traceMatrix[currentPos] = '^';
            }
        }
    }

    score = maximum;

    ret->traceMatrix = traceMatrix;
    ret->scoreMatrix = scoreMatrix;
    ret->score = score;
    
    return ret;
}

struct retVal* smithWaterman(int *subMat, int gapPenalty, char *seq1, char *seq2)
{
    int seq1Pos, seq2Pos, diagUp, diagDown, up, left, maximum, score, currentPos;
    int seq1Len = strlen(seq1) + 1;
    int seq2Len = strlen(seq2) + 1;
    int matLen = seq1Len * seq2Len;
    int *scoreMatrix = (int *) calloc(matLen, sizeof(int));
    struct retVal *ret = (struct retVal*) calloc(1, sizeof(struct retVal));
    char *traceMatrix = (char *) calloc(matLen, sizeof(char));

    score = 0;

    //initializing score matrix
    for (seq1Pos = 0; seq1Pos < seq1Len; seq1Pos++)
    {
        scoreMatrix[seq1Pos] = 0;
        scoreMatrix[seq1Pos * seq1Len] = 0;
        traceMatrix[seq1Pos] = 'x';
        traceMatrix[seq1Pos * seq1Len] = 'x';
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
            currentPos = seq1Pos + (seq2Pos * seq1Len);

            diagUp = scoreMatrix[currentPos - (seq1Len + 1)] + getPairValue(seq1[seq1Pos - 1], seq2[seq2Pos - 1], subMat);

            diagDown = 0; 
            
            left = scoreMatrix[currentPos - 1] + gapPenalty;

            up = scoreMatrix[currentPos - seq1Len] + gapPenalty;

            maximum = max(max(max(diagUp, diagDown), left), up);

            scoreMatrix[currentPos] = maximum;

            if (diagUp == maximum)
            {
                traceMatrix[currentPos] = '\\';
            }
            else if (diagDown == maximum)
            {
                //DIAGONAL UP LOOKS THE SAME AS DIAGONAL DOWN SO I USED '>' INSTEAD
                traceMatrix[currentPos] = '>';
            }
            else if (up == maximum)
            {
                traceMatrix[currentPos] = '^';
            }
            else if (left == maximum)
            {
                traceMatrix[currentPos] = '<';
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

char* randSequence(int num)
{
    int pos, tmp;
    char *sequence = (char *) calloc(num + 1, sizeof(char));
    char letter;

    for (pos = 0; pos < num; pos++)
    {

        tmp = rand() % 4;

        if (tmp == 0)
        {
            letter = 'A';
        }
        else if (tmp == 1)
        {
            letter = 'C';
        }
        else if (tmp == 2)
        {
            letter = 'T';
        }
        else if (tmp == 3)
        {
            letter = 'G';
        }

        sequence[pos] = letter;
    }

    sequence[pos] = '\0';

    return sequence;
}

void generateRandomSequence()
{
    //generates random sequence and outputs to file
    char *seq1 = randSequence(100);
    char *seq2 = randSequence(100);

    FILE *seq1File = fopen("seq1.txt", "w");
    FILE *seq2File = fopen("seq2.txt", "w");

    fprintf(seq1File, "%s\n", seq1);
    fprintf(seq2File, "%s\n", seq2);

    fclose(seq1File);
    fclose(seq2File);
}

int main(int argc, char const *argv[])
{
    double secDiff, usecDiff;
    struct timeval start, end;
    int subMat[16] = {
        2, -1, -1, -1, 
        -1, 2, -1, -1, 
        -1, -1, 2, -1, 
        -1, -1, -1, 2,
        };

    //generateRandomSequence();  

    //only off by a handful now

    FILE *seq1File = fopen("seq1.txt", "r");
    FILE *seq2File = fopen("seq2.txt", "r");

    char *seq1, *seq2;
    seq1 = (char *) calloc(101, sizeof(char));
    seq2 = (char *) calloc(101, sizeof(char));
    fgets(seq1, 101, seq1File);
    fgets(seq2, 101, seq2File);

    fclose(seq1File);
    fclose(seq2File);

    gettimeofday(&start, NULL);
    struct retVal *new = needlemanWunsch(subMat, -2, seq1, seq2);
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

    gettimeofday(&start, NULL);
    struct retVal *new2 = smithWaterman(subMat, -2, seq1, seq2);
    gettimeofday(&end, NULL);

    secDiff = end.tv_sec - start.tv_sec;
    usecDiff = end.tv_usec - start.tv_usec;

    if (usecDiff < 0)
    {
        usecDiff += 1000000;
        secDiff--;
    }

    printf("Score: %d\n", new2->score);
    printf("Time: %lf\n", secDiff + (usecDiff/1000000));

    /* for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)\
        {
            printf("%d ", new->scoreMatrix[j + (i * 100)]);
        }
        printf("\n");
    } */

    free(new2->traceMatrix);
    free(new2->scoreMatrix);
    free(new2);
    free(new->traceMatrix);
    free(new->scoreMatrix);
    free(seq1);
    free(seq2);
    free(new);

    return 0;
}

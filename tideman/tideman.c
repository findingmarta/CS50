#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];
//
bool matrix[MAX][MAX];


// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
int strenght(int i);rm vo
bool cyclicGraph(bool visited[], bool onStack[]);
bool hasCycle(int x, bool visited[], bool onStack[]);


int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }
    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int temp_winner, temp_loser, max;

    for (int i = 0; i < pair_count - 1; i++)
    {
        max = i;
        for (int j = i + 1; j < pair_count; j++)
        {
            if (strenght(j) > strenght(max))
            {
                max = j;
            }
        }
        // Swap
        temp_winner = pairs[max].winner;
        temp_loser = pairs[max].loser;

        pairs[max].winner = pairs[i].winner;
        pairs[max].loser = pairs[i].loser;

        pairs[i].winner = temp_winner;
        pairs[i].loser = temp_loser;
    }
    return;
}

// Calculates the strenght of given pairs
int strenght(int i)
{
    return preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;

        bool visited[MAX];
        bool onStack[MAX];
        //Inicialize
        memset(visited, false, sizeof visited);
        memset(onStack, false, sizeof onStack);

        if (cyclicGraph(visited, onStack))
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int j;
    for (int i = 0; i < candidate_count; i++)
    {
        for (j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                j = candidate_count;
            }
        }
        if (j == candidate_count)
        {
            printf("%s\n", candidates[i]);
        }
        j = 0;
    }
    return;
}

bool cyclicGraph(bool visited[], bool onStack[])
{
    for (int i = 0; i < MAX; i++)
    {
        //Run the DFS algorithm from unvisited vertices
        if (!visited[i] && hasCycle(i, visited, onStack))
        {
            //Forms a cycle
            return true;
        }
    }
    return false;
}

bool hasCycle(int x, bool visited[], bool onStack[])
{
    // we push this 'x' node onto the stack
    onStack[x] = true;
    for (int i = 0; i < MAX; i++)
    {
        if (locked[x][i])       //x's children are only i's holding True values in array "locked[x][i]"
        {
            if (onStack[i])
            {
                return true;
            }
            if (!visited[i] && hasCycle(i, visited, onStack))
            {
                return true;
            }
        }
    }
    //we pop the 'x' from the stack and mark it as visited
    onStack[x] = false;
    visited[x] = true;
    return false;
}
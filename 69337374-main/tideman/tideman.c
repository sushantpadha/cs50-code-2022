#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

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
int pair_strength(int i);
void lock_pairs(void);
bool check_path(int from, int to);
void print_winner(void);

void print_arr(int arr[], int length);
void print_candidates(void);
void print_locked(void);

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
    // iterate over all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // if candidate string matches name
        if (strcmp(candidates[i], name) == 0)
        {
            // set ranks[rank] value to candidate index
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // iterate over all ranked candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // iterate over all lower ranked candidates
        for (int j = i + 1; j < candidate_count; j++)
        {
            // i > j, so candidate ranks[i] is more preferred
            // than candidate ranks[j]
            int winner = ranks[i];
            int loser = ranks[j];
            preferences[winner][loser]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // iterate over all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // iterate over all candidates after i
        // this is done so that we don't add a pair twice
        for (int j = i + 1; j < candidate_count; j++)
        {
            // if candidate i is preferred over j
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            // if candidate j is preferred over i
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // strength of victory is calculated for a pair of candidates
    // pairs already sorted
    int n = 0;
    // iterate pair_count times
    for (int i = 0; i < pair_count; i++)
    {
        // index of highest strength pair seen so far
        int max = n;
        // find the pair with the highest victory strength
        for (int j = n; j < pair_count; j++)
        {
            // if this pair has higher strength, update max
            if (pair_strength(j) > pair_strength(max))
            {
                max = j;
            }
        }
        // swap pairs indexed n and max
        pair a = pairs[n];
        pairs[n] = pairs[max];
        pairs[max] = a;
        // increment n
        n++;
    }
    return;
}

int pair_strength(int i)
{
    pair p = pairs[i];
    // strength of pair is the preference of its winner of its loser
    return preferences[p.winner][p.loser];
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{

    // iterate over all pairs
    for (int i = 0; i < pair_count; i++)
    {
        // get pair
        pair p = pairs[i];
        // check if this pair creates cyclic graph
        if (!(check_path(p.winner, p.loser) || check_path(p.loser, p.winner)))
        {
            // change this pair to true
            locked[p.winner][p.loser] = true;
        }
    }
    return;
}

// Return whether there exists a path from `from` to `to` directionally
bool check_path(int from, int to)
{
    // if `from` -> `to` exists, return true
    if (locked[from][to])
    {
        return true;
    }
    // or, go through all other candidates, find one that `from` does connect to
    // and recursively check all of its connected nodes until we find `to`
    // basically DFS in a graph
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[from][i])
        {
            if (check_path(i, to))
            {
                return true;
            }
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // the winner (or source) will have no incoming nodes
    // if node i is the winner, the ith column will only have false entries

    // iterate over all columns
    for (int i = 0; i < candidate_count; i++)
    {
        // store sum of true entries in this column (or incoming edges)
        int incoming = 0;
        // iterate over all rows
        for (int j = 0; j < candidate_count; j++)
        {
            // note the order of indexing (i is the col, j is the row)
            if (locked[j][i])
            {
                incoming++;
            }
        }
        // if incoming is still zero, this must be the winner
        if (incoming == 0)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
    return;
}

void print_candidates(void)
{
    printf("candidates = ");
    printf("[");
    for (int i = 0; i < candidate_count; i++)
    {
        printf("%i ", i);
    }
    printf("\b]\n");
}

void print_arr(int arr[], int length)
{
    printf("[");
    for (int i = 0; i < length; i++)
    {
        printf("%i ", arr[i]);
    }
    printf("\b]\n");
}

void print_locked(void)
{
    printf("locked = \n");
    printf("[");
    for (int i = 0; i < candidate_count; i++)
    {
        if (i != 0)
        {
            printf(" ");
        }
        for (int j = 0; j < candidate_count; j++)
        {
            int c = (int)locked[i][j];
            printf("%i  ", c);
        }
        if (i != candidate_count - 1)
        {
            printf("\n");
        }
    }
    printf("\b\b]\n");
}

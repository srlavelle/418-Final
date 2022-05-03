// This code adapted from https://rosettacode.org/wiki/Solve_a_Numbrix_puzzle#C.2B.2B

#include <vector>
#include <sstream>
#include <iostream>
#include <iterator>
#include <cstdlib>
#include <string>
#include <bitset>
#include <chrono>

#include <omp.h>
#include <getopt.h>

using namespace std;
typedef bitset<4> hood_t;

struct node
{
	int val;
	hood_t neighbors;
};

class nSolver
{
public:

    // initiates solving; called from main function
	void solve(vector<string>& puzz, int max_wid, int num_threads)
	{
		if (puzz.size() < 1) return;
		wid = max_wid;
		hei = static_cast<int>(puzz.size()) / wid;
		max = wid * hei;
		int len = max, c = 0;
		arr = vector<node>(len, node({ 0, 0 })); // holds solution
		weHave = vector<bool>(len + 1, false); // hold whether or not that position is solved

		for (const auto& s : puzz) // go thru and mark what values are already there
		{
			if (s == "*") { max--; arr[c++].val = -1; continue; }
			arr[c].val = atoi(s.c_str());
			if (arr[c].val > 0) weHave[arr[c].val] = true;
			c++;
		}

		solveIt(num_threads); // does the solving
        c = 0;
		for (auto&& s : puzz) // fill in all the unknowns from the solution in arr
		{
			if (s == ".")
				s = std::to_string(arr[c].val);
			c++;
		}
	}

private:

    // search function that I tried a different method of parallelizing in:
    // I parallelized the data calculations done before the recursive calls,
    // but left the recursion itself alone. This also did not yield speedup.
    bool search(int x, int y, int w, int dr, int num_threads)
	{
        if ((w > max && dr > 0) || (w < 1 && dr < 0) || (w == max && weHave[w])) {
            //printf("Puzzle Complete!\n");
            return true;
        }

		node& n = arr[x + y * wid];
		n.neighbors = getNeighbors(x, y, num_threads);

        bool* is_open_spot = (bool*)malloc(4 * sizeof(bool));
        int* a = (int*)malloc(4 * sizeof(int));
        int* b = (int*)malloc(4 * sizeof(int));
        bool* next_value_found = (bool*)malloc(4 * sizeof(bool));

        if (weHave[w])
		{
		    //begin parallel section
            omp_set_num_threads(num_threads);
            #pragma omp parallel for shared(next_value_found, a, b) schedule(dynamic)
            for (int d = 0; d < 4; d++)
			{
                a[d] = x + dx[d];
                b[d] = y + dy[d];
				if (n.neighbors[d] && arr[a[d] + b[d] * wid].val == w)
				{
                    next_value_found[d] = true;
				}
                else
                {
                    next_value_found[d] = false;
                }
			} // end parallel section

            for (int d = 0; d < 4; d++)
            {
                if (next_value_found[d] && search(a[d], b[d], w + dr, dr, num_threads))
                {

                    free(next_value_found);
                    free(is_open_spot);
                    free(a);
                    free(b);

                    return true;
                }
            }

        free(next_value_found);
        free(is_open_spot);
        free(a);
        free(b);

		return false;
		}

        // w is not already in the puzzle


        // begin parallel section
        omp_set_num_threads(num_threads);
        #pragma omp parallel for shared(is_open_spot, a, b) schedule(dynamic)
        for (int d = 0; d < 4; d++)
        {
            a[d] = x + dx[d];
            b[d] = y + dy[d];
			if (n.neighbors[d] && arr[a[d] + b[d] * wid].val == 0) {
                is_open_spot[d] = true;
                //printf("true sent\n");
            }
            else {
                is_open_spot[d] = false; }

        } // end parallel section

        for (int d = 0; d < 4; d++)
        {
            if (is_open_spot[d] == true)
            {
                //printf("true recvd\n");
			    arr[a[d] + b[d] * wid].val = w;
			    if (search(a[d], b[d], w + dr, dr, num_threads)) {
				    free(is_open_spot);
                    free(a);
                    free(b);
                    return true;
                }
			    arr[a[d] + b[d] * wid].val = 0;
            }
		}

        free(next_value_found);
        free(is_open_spot);
        free(a);
        free(b);

		return false;
	}

    // search function with parallelized recursion, executed via tasks
    bool task_search(int x, int y, int w, int dr, vector<node> local_arr, int depth)
	{

        if ((w > max && dr > 0) || (w < 1 && dr < 0) || (w == max && weHave[w]))
        {
            // copy local arr to arr
            for (int i = 0; i < max; i++) {
                arr[i] = local_arr[i];
            }

            return true;
        }

		node& n = local_arr[x + y * wid];
		n.neighbors = getNeighbors(x, y, 1);
		if (weHave[w])
		{
			for (int d = 0; d < 4; d++)
			{
				if (n.neighbors[d])
				{
					int a = x + dx[d], b = y + dy[d];
					if (local_arr[a + b * wid].val == w)
						if (task_search(a, b, w + dr, dr, local_arr, depth+1))

							return true;
				}
			}
			return false;
		}

        int res = 0;
        bool valid_path = false;
		for (int d = 0; d < 4; d++)
		{
            omp_set_nested(1);
            #pragma omp task shared(n, res) private(valid_path) firstprivate(local_arr)
            {
                //if (w == 2)
                //    printf("##### %d #####\n", omp_get_thread_num());
			    if (n.neighbors[d])
			    {
				    int a = x + dx[d], b = y + dy[d];
				    if (local_arr[a + b * wid].val == 0)
				    {

				        local_arr[a + b * wid].val = w;

                        if (depth < 8) {
                            valid_path = task_search(a, b, w + dr, dr, local_arr, depth+1); }
                        else {
                            valid_path = old_search(a, b, w + dr, dr, local_arr); }


					    if (valid_path) {
						    //return true;
                            //break;
                            res = d;
                        }
				    }
			    }
            }
		}
        #pragma omp taskwait


        if (res != 0) {
            int a = x + dx[res], b = y + dy[res];
            local_arr[a + b * wid].val = w;
            return true;
        }

		return false;
	}

    // original, non-parallelized search function
    bool old_search(int x, int y, int w, int dr, vector<node> local_arr)
	{

        if ((w > max && dr > 0) || (w < 1 && dr < 0) || (w == max && weHave[w])) // return true;
        {
            // copy local arr to arr
            for (int i = 0; i < max; i++) {
                arr[i] = local_arr[i];
            }

            return true;
        }


		node& n = local_arr[x + y * wid];
		n.neighbors = getNeighbors(x, y, 1);
		if (weHave[w])
		{
			for (int d = 0; d < 4; d++)
			{
				if (n.neighbors[d])
				{
					int a = x + dx[d], b = y + dy[d];
					if (local_arr[a + b * wid].val == w)
						if (old_search(a, b, w + dr, dr, local_arr))

							return true;
				}
			}
			return false;
		}

		for (int d = 0; d < 4; d++)
		{
			if (n.neighbors[d])
			{
				int a = x + dx[d], b = y + dy[d];
				if (local_arr[a + b * wid].val == 0)
				{
					local_arr[a + b * wid].val = w;
					if (old_search(a, b, w + dr, dr, local_arr))
						return true;
					local_arr[a + b * wid].val = 0;
				}
			}
		}
		return false;
	}

    // determines if the four surrounding spaces are valid spaces on the board.
	hood_t getNeighbors(int x, int y, int num_threads)
	{
		hood_t retval;
        int a;
        int b;
        bool out_of_bounds;

		for (int xx = 0; xx < 4; xx++)
		{
			a = x + dx[xx];
            b = y + dy[xx];
			out_of_bounds = (a < 0 || b < 0 || a >= wid || b >= hei); // if out of bounds of grid
				//continue;
			if (!out_of_bounds)
            {
                if (arr[a + b * wid].val > -1)
                {
				    retval.set(xx);
                }
            }
		}

        return retval;
	}

    // where the recursive solving function is called from.
	void solveIt(int num_threads)
	{
		int x, y, z;
        findStart(x, y, z); // set x & y to coords of lowest value. z holds this value
		if (z == 99999) { cout << "\nCan't find start point!\n"; return; }

        omp_set_num_threads(num_threads);
        #pragma omp parallel
        #pragma omp single
        {
        task_search(x, y, z + 1, 1, arr, 0);
        }

        //search(x, y, z + 1, 1, num_threads); // recursively finds all numbers > z
		//if (z > 1) search(x, y, z - 1, -1, num_threads); // recursively finds all numbers < z
	}

    // goes through all values in puzzle to find lowest value.
    // this is the starting point.
	void findStart(int& x, int& y, int& z)
	{
		z = 99999;
		for (int b = 0; b < hei; b++)
		for (int a = 0; a < wid; a++)
		if (arr[a + wid * b].val > 0 && arr[a + wid * b].val < z)
		{
			x = a; y = b;
			z = arr[a + wid * b].val;
		}

	}

	vector<int> dx = vector<int>({ -1, 1, 0, 0 });
	vector<int> dy = vector<int>({ 0, 0, -1, 1 });
	int wid, hei, max;
	vector<node> arr;
	vector<bool> weHave;
};

//------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    // timing code modeled after that for Assignment 3
    using namespace std::chrono;
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::duration<double> dsec;


    int num_threads = 1; //default
    int opt;
    while ((opt = getopt(argc, argv, "t:")) != -1) {
        switch (opt) {
        case 't':
            num_threads = atoi(optarg);
            break;
        }
    }

	int wid;
    string p;
    string p1;
    string p2;
    string p3;
    string p4;
    string p5;
    string p6;


	p1 =".  .  .  .  .  .  .  .  . \
         .  . 46 45  . 55 74  .  . \
         . 38  .  . 43  .  . 78  . \
         . 35  .  .  .  .  . 71  . \
         .  . 33  .  .  . 59  .  . \
         . 17  .  .  .  .  . 67  . \
         . 18  .  . 11  .  . 64  . \
         .  . 24 21  .  1  2  .  . \
         .  .  .  .  .  .  .  .  ."; int wid1 = 9;

    p2 =".  .  .  .  .  .  .  .  . \
         . 11 12 15 18 21 62 61  . \
         .  6  .  .  .  .  . 60  . \
         . 33  .  .  .  .  . 57  . \
         . 32  .  .  .  .  . 56  . \
         . 37  .  1  .  .  . 73  . \
         . 38  .  .  .  .  . 72  . \
         . 43 44 47 48 51 76 77  . \
         .  .  .  .  .  .  .  .  ."; int wid2 = 9;

    p3 ="17  .  .  . 11  .  .  . 59 \
          . 15  .  .  6  .  . 61  . \
          .  .  3  .  .  . 63  .  . \
          .  .  .  . 66  .  .  .  . \
         23 24  . 68 67 78  . 54 55 \
          .  .  .  . 72  .  .  .  . \
          .  . 35  .  .  . 49  .  . \
          . 29  .  . 40  .  . 47  . \
         31  .  .  . 39  .  .  . 45"; int wid3 = 9;

    // This is the example puzzle from the Numbrix website!
    p4 =" .  .  .  .  .  .  .  .  . \
          . 25  . 31  . 69  . 73  . \
          .  . 19  . 35  . 79  .  . \
          . 21  .  .  .  .  . 61  . \
          .  . 15  .  .  . 81  .  . \
          .  9  .  .  .  .  . 63  . \
          .  .  7  . 45  . 51  .  . \
          .  1  . 41  . 47  . 53  . \
          .  .  .  .  .  .  .  .  ."; int wid4 = 9;

    p5 ="53  52   .   .  47   .   .   .   .   . 211   .   . 202 201 \
         54  51   .   .   .   .   .  21   .   .   .   .   . 203 200 \
          .   .   .  44   .   .   .   .   .   .   . 216   .   .   . \
          .   .  60   .   .  39   .  23   . 219   .   . 206   .   . \
         63   .   .   .   .  40  29  24  15  10   .   .   .   . 197 \
          .   .   .  69  70  71   .   .   .  11   8 225   .   .   . \
          .   .   .   .  73   .  27   .  13   .   7   .   1   .   . \
          .  77   .  83  84   .   .  89   .   .  92   5   . 191   . \
          .   .   .   .  85   .  99   .  95   .  93   .   .   .   . \
          .   .   . 119 118 101   .   .   . 165 166 169   .   .   . \
        129   .   .   .   . 102 103 104 105 164   .   .   .   . 185 \
          .   . 124   .   . 115   . 111   . 163   .   . 172   .   . \
          .   .   . 142   .   .   .   .   .   .   . 160   .   .   . \
        136 135   .   .   .   .   . 109   .   .   .   .   . 179 178 \
        137 138   .   . 145   .   .   .   .   . 153   .   . 176 177"; int wid5 = 15;

    p6 =" 1   . \
          .   3"; int wid6 = 2;


    // choose puzzle to solve
    p = p2; // maybe make command-line arg later
    wid = wid2;

    istringstream iss(p);
    vector<string> puzz;
	copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(puzz));

    // first print unsolved puzzle
    int c = 0; // tracks position in row
	for (const auto& s : puzz)
	{
		if (s != "*" && s != ".")
		{
			if (atoi(s.c_str()) < 10) cout << " ";
            if (atoi(s.c_str()) < 100) cout << " ";
			cout << s << " ";
		}
		else cout << "___ ";
		if (++c >= wid) { cout << endl; c = 0; }
	}
	cout << endl;


    nSolver s;

    // start time
    auto compute_start = Clock::now();
    double compute_time = 0;

    s.solve(puzz, wid, num_threads);
    // end time
    compute_time += duration_cast<dsec>(Clock::now() - compute_start).count();

    cout << "Solved!\n";
    cout << "Computation time: " << compute_time*1000 << " ms\n\n";

    // print solved puzzle
	c = 0;
	for (const auto& s : puzz)
	{
		if (s != "*" && s != ".")
		{
			if (atoi(s.c_str()) < 10) cout << " ";
            if (atoi(s.c_str()) < 100) cout << " ";
			cout << s << " ";
		}
		else cout << "   ";
		if (++c >= wid) { cout << endl; c = 0; }
	}

    return 1; // indicating success
}

// This code adapted from https://rosettacode.org/wiki/Solve_a_Numbrix_puzzle#C.2B.2B

#include <vector>
#include <sstream>
#include <iostream>
#include <iterator>
#include <cstdlib>
#include <string>
#include <bitset>
#include <chrono>

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

	void solve(vector<string>& puzz, int max_wid)
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

		solveIt(); // does the solving
        c = 0;
		for (auto&& s : puzz) // fill in all the unknowns from the solution in arr
		{
			if (s == ".")
				s = std::to_string(arr[c].val);
			c++;
		}
	}

private:
	bool search(int x, int y, int w, int dr)
	{
		if ((w > max && dr > 0) || (w < 1 && dr < 0) || (w == max && weHave[w])) return true;

		node& n = arr[x + y * wid];
		n.neighbors = getNeighbors(x, y);
		if (weHave[w])
		{
			for (int d = 0; d < 4; d++)
			{
				if (n.neighbors[d])
				{
					int a = x + dx[d], b = y + dy[d];
					if (arr[a + b * wid].val == w)
						if (search(a, b, w + dr, dr))
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
				if (arr[a + b * wid].val == 0)
				{
					arr[a + b * wid].val = w;
					if (search(a, b, w + dr, dr))
						return true;
					arr[a + b * wid].val = 0;
				}
			}
		}
		return false;
	}

	hood_t getNeighbors(int x, int y)
	{
		hood_t retval;
		for (int xx = 0; xx < 4; xx++)
		{
			int a = x + dx[xx], b = y + dy[xx];
			if (a < 0 || b < 0 || a >= wid || b >= hei) // if out of bounds of grid
				continue;
			if (arr[a + b * wid].val > -1)
				retval.set(xx);
		}
		return retval;
	}

	void solveIt()
	{
		int x, y, z;
        findStart(x, y, z); // set x & y to coords of lowest value. z holds this value
		if (z == 99999) { cout << "\nCan't find start point!\n"; return; }
		search(x, y, z + 1, 1); // recursively finds all numbers > z
		if (z > 1) search(x, y, z - 1, -1); // recursively finds all numbers < z
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

	int wid;
    string p;
    string p1;
    string p2;
    string p3;
    string p4;
    string p5;


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


    // choose puzzle to solve
    p = p1; // maybe make command-line arg later
    wid = wid1;

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

    s.solve(puzz, wid);
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

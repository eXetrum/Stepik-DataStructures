#include <iostream>
#include <vector>
#include <string>
using namespace std;



class rolling_hash {   
    const long long P = 1000000007;
    const long long X = 263;
private:    
    

    long long hash(const string& S, long long m) const {
        long long sum = 0;
        long long x = 1;
        for (long long i = 0; i < m; ++i) {
            sum = (X * sum + S[i]) % P;
        }
        return sum;
    }

    long long roll(long long prev_hash, char prev_char, char new_char) {
        /*previous_hash = ((previous_hash - prev_char * multiplier) * D + new_char) % P;
        if (previous_hash < 0) previous_hash = previous_hash + P;
        return previous_hash;*/

        prev_hash += P;
        prev_hash = (prev_hash - (prev_char * max_factor) % P) % P;
        prev_hash = (prev_hash * X) % P;
        return (prev_hash + new_char) % P;
    }

protected:
    string pattern;
    string text;
    long long m;
    long long cursor_pos;
    long long max_factor;

    long long pattern_hash;    
    long long cur_substr_hash;

    
    bool is_pattern_match_window() const {
        for (long long i = 0; i < m; ++i) {
            if (pattern[i] != text[cursor_pos + i]) return false;
        }
        return true;
    }
public:
    rolling_hash(const string& pattern, const string& text) {
        reset(pattern, text);
    }

    void reset(const string& pattern, const string& text) {
        this->pattern = pattern;
        this->text = text;
        m = pattern.size();
        cursor_pos = 0;
        max_factor = 1;

        pattern_hash = hash(pattern, m);
        cur_substr_hash = hash(text.substr(0, m), m);
        for (long long i = 0; i < m - 1; ++i) {
            max_factor = (max_factor * X) % P;
        }
    }

    long long get_cursor_pos() const { return cursor_pos; }
    
    bool can_move_cursor() const { return cursor_pos < text.size() - m + 1; }
    
    void next() {
        bool verbose = false;

        if (pattern_hash == cur_substr_hash 
            && is_pattern_match_window()) cout << cursor_pos << endl;

        if(verbose)
            cout << "Pattern h=" << pattern_hash << ", substr direct hash=" << hash(text.substr(cursor_pos, m), m) << ", substr rolling hash=" << cur_substr_hash << ", substr=" << text.substr(cursor_pos, m) << endl;

        // Shift window, and recalculate hash
        if (cursor_pos + m < text.size()) {
            cur_substr_hash = roll(cur_substr_hash, text[cursor_pos], text[cursor_pos + m]);
        }
        ++cursor_pos;
    }
};

int main() {
    string pattern, text;

    cin >> pattern >> text;    

    rolling_hash hs(pattern, text);
    while (hs.can_move_cursor()) {
        hs.next();
    }

    return 0;
}
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <iomanip>
using namespace std;


struct packet_t {
    int id;
    int arrive;
    int process;
    // Actual start time
    int start_time;
    // Finish time
    int finish_time;

    packet_t()
        : id(-1), arrive(-1), process(-1), start_time(-1), finish_time(-1) { }

    packet_t(const packet_t& p)
        : id(p.id), arrive(p.arrive), process(p.process), start_time(p.start_time), finish_time(p.finish_time) { }

    packet_t& operator=(const packet_t& rhs) {
        if (this != &rhs) {
            id = rhs.id;
            arrive = rhs.arrive;
            process = rhs.process;
            start_time = rhs.start_time;
            finish_time = rhs.finish_time;
        }
        return *this;
    }
    bool operator ==(const packet_t rhs) const { return id == rhs.id; }
    bool operator !=(const packet_t rhs) const { return id != rhs.id; }
};

enum event_type {
    PACKET_ARRIVE_ACCEPT,       // Append to the buffer
    PACKET_ARRIVE_DROP,         // Buffer is full, drop
    PACKET_PROCESSING_START,    // Send packet from buffer to CPU for work
    PACKET_PROCESSING_END,      // Packet processed by CPU
    DEFAULT                     // Nothing hpnd
};


class buffer_simulator_t {
    int max_buff_size;
    queue<packet_t*> buff;

    // Current clock time
    int clock_time;
    //  
    int cur_packet_idx;
    // The current packet being processed on the CPU
    packet_t* cpu_packet;

    vector<packet_t>* packets;
    bool verbose;
private:
    buffer_simulator_t(const buffer_simulator_t&) = delete;
    buffer_simulator_t& operator=(const buffer_simulator_t&) = delete;

private:
    void reset(int max_buff_size, vector<packet_t>& packets, bool verbose) {
        this->max_buff_size = max_buff_size;
        this->clock_time = 0;
        this->cur_packet_idx = 0;
        this->cpu_packet = nullptr;
        this->packets = &packets;
        this->verbose = verbose;
    }

    void print_header() const {
        if (!verbose) return;
        cout << " | " << setw(14) << right << "Event";
        cout << " | " << setw(8) << right << "CPU";
        cout << " | " << setw(4) << right << "Time";
        cout << " | " << setw(4) << right << "buff";
        cout << " | " << setw(20) << left << "packet";
        cout << endl;
        cout << string(60, '-') << endl;
    }

    void print_event_summary(event_type e) {
        if (!verbose) return;

        if (cpu_is_bussy())
            cout << " | " << setw(8) << right << "burst";
        else
            cout << " | " << setw(8) << right << "idle";

        cout << " | " << setw(4) << right << clock_time;
        cout << " | " << setw(4) << right << buff.size();

        switch (e)
        {
        case PACKET_ARRIVE_ACCEPT:
            cout << " | " << "[+] id = " << packets->at(cur_packet_idx).id;
            break;
        case PACKET_ARRIVE_DROP:
            cout << " | " << "[-]:drop id = " << packets->at(cur_packet_idx).id;
            break;
        case PACKET_PROCESSING_START:
            cout << " | " << setw(20) << left;
            cout << "<=IN id=" + to_string(cpu_packet->id) + ", start=" + to_string(cpu_packet->start_time)
                + "(" + to_string(cpu_packet->arrive) + ")" + ", end=" + to_string(cpu_packet->finish_time);
            break;
        case PACKET_PROCESSING_END:
            cout << " | " << setw(20) << left;
            cout << "=>OUT id=" + to_string(cpu_packet->id) + ", start=" + to_string(cpu_packet->start_time)
                + "(" + to_string(cpu_packet->arrive) + ")" + ", end=" + to_string(cpu_packet->finish_time);
            break;
        default:
            cout << " | " << setw(20) << left;
            break;
        }
        cout << endl;
        cout << string(60, '-') << endl;
    }

    bool cpu_is_bussy() const { return cpu_packet != nullptr; }
    bool is_packet_serve_done() const { return cpu_packet != nullptr && clock_time == cpu_packet->finish_time; }
    bool is_new_packet_arrive() const { return cur_packet_idx < packets->size() && clock_time == packets->at(cur_packet_idx).arrive; }
public:
    buffer_simulator_t()
        : max_buff_size(0),
        clock_time(0), cur_packet_idx(0), cpu_packet(nullptr),
        packets(nullptr), verbose(false) { }

    void run(int max_buff_size, vector<packet_t>& packets, bool verbose = false) {
        reset(max_buff_size, packets, verbose);
        print_header();

        while (cur_packet_idx < packets.size() || !buff.empty()) {
            // check if CPU work done
            if (is_packet_serve_done()) {
                print_event_summary(PACKET_PROCESSING_END);
                // Drop from buffer
                if (*buff.front() == *cpu_packet) {
                    buff.pop();
                    cpu_packet = nullptr;
                }
            }
            else if (!cpu_is_bussy() && !buff.empty()) { // CPU in IDLE state and buffer is not empty, thus we can send some work to CPU
                cpu_packet = buff.front();
                cpu_packet->start_time = clock_time;
                cpu_packet->finish_time = clock_time + cpu_packet->process;
                print_event_summary(PACKET_PROCESSING_START);
            }
            else if (is_new_packet_arrive()) {
                if (buff.size() < max_buff_size) {
                    print_event_summary(PACKET_ARRIVE_ACCEPT);
                    buff.push(&packets.at(cur_packet_idx));
                }
                else {
                    print_event_summary(PACKET_ARRIVE_DROP);
                }

                // Next packet index
                ++cur_packet_idx;
            }
            else {
                print_event_summary(DEFAULT);
                ++clock_time;
            }
        }

        // Timings for each packet
        for (int i = 0; i < packets.size(); ++i) {
            cout << packets[i].start_time << endl;
        }
    }
};

int main() {
    int max_buff_size, n;
    cin >> max_buff_size >> n;
    queue<packet_t*> buff;
    vector<packet_t> packets(n);

    for (int i = 0; i < n; ++i) {
        cin >> packets[i].arrive >> packets[i].process;
        packets[i].id = i + 1;
    }

    buffer_simulator_t simulator;
    simulator.run(max_buff_size, packets);
    return 0;
}
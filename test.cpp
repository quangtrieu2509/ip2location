#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>

#define PATH "IP2LOCATION-LITE-DB11.CSV/IP2LOCATION-LITE-DB11.csv"

using namespace std;
using namespace std::chrono;


struct IPv4{
  unsigned long int from, to;
  string country;
};

unsigned int ipv4ToNumber(const string& ip) {
    unsigned int result = 0;
    unsigned int byte;
    int shift = 24;
    
    istringstream ipStream(ip);
    while (ipStream >> byte) {
        result |= (byte << shift);
        shift -= 8;
        if (ipStream.peek() == '.') ipStream.ignore();
    }
    
    return result;
}

void readData(vector<IPv4>& listIp){
	ifstream file(PATH);
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        return;
    }
    string line;
    
    cout << "Reading file..." << endl;
    
    while (getline(file, line)) {
        istringstream ss(line);
        string row;

        IPv4 ip;

        getline(ss, row, ',');
        ip.from = stoul(row);

        getline(ss, row, ',');
        ip.to = stoul(row);

        getline(ss, row, ',');
        getline(ss, ip.country, ',');   
        
        listIp.push_back(move(ip));
    }
    file.close();
}

int binarySearch(vector<IPv4>& listIp, int l, int r, int x)
{
    if (r >= l) {
        int mid = l + (r - l) / 2;
 
        if (x >= listIp[mid].from && x <= listIp[mid].to)
            return mid;
        else if (x < listIp[mid].from)
            return binarySearch(listIp, l, mid - 1, x);
        else
            return binarySearch(listIp, mid + 1, r, x);
    }
    
    return -1;
}


int main() {
	vector<IPv4> listIp;
	readData(listIp);
	
	auto start = high_resolution_clock::now();
    
	const unsigned long int ipnum = ipv4ToNumber("8.8.8.8"); 
	const int order = binarySearch(listIp, 0, listIp.size()-1, ipnum);
	
	auto stop = high_resolution_clock::now();
	
	cout << "IP to number: " << ipnum << endl;
	cout << "Order: " << order << endl;
	cout << "Result: " << listIp[order].country << endl;
	
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken: "
         << duration.count() << " microseconds" << endl;
    return 0;
}


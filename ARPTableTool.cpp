/*********************************************************************************************************************************
A GUI tool to display modify and remove IP-to-Physical address translation tables used by the Address Resolution Protocol (ARP).
**********************************************************************************************************************************/
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>

using namespace std;

// Structure to store ARP table entry
struct ARPEntry 
{
    string ipAddress;
    string physicalAddress;
};

// Function to get the ARP table
vector<ARPEntry> getARPTable() 
{
    vector<ARPEntry> arpTable;
    FILE* pipe = popen("arp -a", "r");
    if (!pipe) 
    {
        cerr << "Failed to run arp command" << endl;
        return arpTable;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), pipe)) 
    {
        char* ipAddr = strtok(buffer, " ");
        if (ipAddr && strcmp(ipAddr, "Address") != 0) 
        {
            char* physAddr = strtok(NULL, " ");
            if (physAddr) 
            {
                ARPEntry entry;
                entry.ipAddress = ipAddr;
                entry.physicalAddress = physAddr;
                arpTable.push_back(entry);
            }
        }
    }

    pclose(pipe);
    return arpTable;
}

// Function to display the ARP table
void displayARPTable(const vector<ARPEntry>& arpTable) 
{
    cout << left << setw(20) << "IP Address" << setw(20) << "Physical Address" << endl;
    cout << setfill('-') << setw(40) << "-" << setfill(' ') << endl;
    for (const auto& entry : arpTable) 
    {
        cout << left << setw(20) << entry.ipAddress << setw(20) << entry.physicalAddress << endl;
    }
}

// Function to add or modify an ARP table entry
void addOrModifyARPEntry(vector<ARPEntry>& arpTable) 
{
    string ipAddress, physicalAddress;
    cout << "Enter IP Address: ";
    cin >> ipAddress;
    cout << "Enter Physical Address: ";
    cin >> physicalAddress;

    bool found = false;
    for (auto& entry : arpTable) 
    {
        if (entry.ipAddress == ipAddress) 
        {
            entry.physicalAddress = physicalAddress;
            found = true;
            break;
        }
    }

    if (!found) 
    {
        ARPEntry newEntry = {ipAddress, physicalAddress};
        arpTable.push_back(newEntry);
    }

    cout << "ARP table entry added/modified successfully." << endl;
}

// Function to remove an ARP table entry
void removeARPEntry(vector<ARPEntry>& arpTable) 
{
    string ipAddress;
    cout << "Enter IP Address to remove: ";
    cin >> ipAddress;

    auto it = arpTable.begin();
    while (it != arpTable.end()) 
    {
        if (it->ipAddress == ipAddress) 
        {
            it = arpTable.erase(it);
            cout << "ARP table entry removed successfully." << endl;
            return;
        }
        ++it;
    }

    cout << "IP Address not found in the ARP table." << endl;
}

int main() 
{
    vector<ARPEntry> arpTable = getARPTable();

    int choice;
    do 
    {
        cout << "\nARP Table Management" << endl;
        cout << "1. Display ARP Table" << endl;
        cout << "2. Add/Modify ARP Entry" << endl;
        cout << "3. Remove ARP Entry" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) 
        {
            case 1:
                displayARPTable(arpTable);
                break;
            case 2:
                addOrModifyARPEntry(arpTable);
                break;
            case 3:
                removeARPEntry(arpTable);
                break;
            case 4:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 4);
    return 0;
}

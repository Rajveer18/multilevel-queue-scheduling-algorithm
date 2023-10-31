#include <iostream>
using namespace std;

struct Process
{
    int priority;
    int burst_time;
    int turnaround_time;
    int remaining_time;
};

struct Queue
{
    int priority_start;
    int priority_end;
    int total_time = 0;
    int length = 0;
    Process *processes;
    bool executed = false;
};

bool isNotComplete(Queue q[])
{
    bool notComplete = false;
    int countInc = 0;
    for (int i = 0; i < 3; i++)
    {
        countInc = 0;
        for (int j = 0; j < q[i].length; j++)
        {
            if (q[i].processes[j].remaining_time != 0)
            {
                notComplete = true;
            }
            else
            {
                countInc += 1;
            }
        }
        if (countInc == q[i].length)
        {
            q[i].executed = true;
        }
    }
    return notComplete;
}

void sortProcessesByPriority(Queue q)
{
    // Sort processes in a queue by priority
    for (int i = 1; i < q.length; i++)
    {
        for (int j = 0; j < q.length - 1; j++)
        {
            if (q.processes[j].priority < q.processes[j + 1].priority)
            {
                Process temp = q.processes[j + 1];
                q.processes[j + 1] = q.processes[j];
                q.processes[j] = temp;
            }
        }
    }
}

void updateTotalTime(Queue q[])
{
    for (int i = 0; i < 3; i++)
    {
        if (q[i].executed == false)
        {
            for (int j = 0; j < q[i].length; j++)
            {
                if (q[i].processes[j].remaining_time != 0)
                {
                    q[i].processes[j].turnaround_time += 1;
                }
            }
            q[i].total_time += 1;
        }
    }
}

int main()
{
    // Initialize 3 queues
    Queue queues[3];
    queues[0].priority_start = 7;
    queues[0].priority_end = 9;
    queues[1].priority_start = 4;
    queues[1].priority_end = 6;
    queues[2].priority_start = 1;
    queues[2].priority_end = 3;

    int num_processes, process_priority, burst_time;

    // Prompt the user to enter the number of processes
    cout << "Enter the number of processes: ";
    cin >> num_processes;
    Process processes[num_processes];

    // Input processes and assign them to respective queues
    for (int i = 0; i < num_processes; i++)
    {
        cout << "Enter the priority of process " << (i + 1) << ": ";
        cin >> process_priority;
        cout << "Enter the burst time of process " << (i + 1) << ": ";
        cin >> burst_time;
        processes[i].priority = process_priority;
        processes[i].burst_time = burst_time;
        processes[i].turnaround_time = burst_time;
        for (int j = 0; j < 3; j++)
        {
            if (queues[j].priority_start <= process_priority && process_priority <= queues[j].priority_end)
            {
                queues[j].length++;
            }
        }
    }

    // Allocate memory for processes in each queue
    for (int i = 0; i < 3; i++)
    {
        int length = queues[i].length;
        queues[i].processes = new Process[length];
    }

    // Assign processes to their respective queues
    int q1_idx = 0, q2_idx = 0, q3_idx = 0;
    for (int i = 0; i < num_processes; i++)
    {
        if (queues[0].priority_start <= processes[i].priority && processes[i].priority <= queues[0].priority_end)
        {
            queues[0].processes[q1_idx++] = processes[i];
        }
        else if (queues[1].priority_start <= processes[i].priority && processes[i].priority <= queues[1].priority_end)
        {
            queues[1].processes[q2_idx++] = processes[i];
        }
        else
        {
            queues[2].processes[q3_idx++] = processes[i];
        }
    }

    q1_idx--;
    q2_idx--;
    q3_idx--;

    // Display the initial state of queues
    for (int i = 0; i < 3; i++)
    {
        cout << "Queue " << (i + 1) << ": ";
        for (int j = 0; j < queues[i].length; j++)
        {
            cout << "P" << (j + 1) << "->";
        }
        cout << "NULL\n";
    }

    int timer = 0;
    int current_queue = -1;
    int rr_timer = 4;
    int counter = 0;
    int counter_ps = 0;
    int counter_fcfs = 0;

    while (isNotComplete(queues))
    {
        if (timer == 10)
        {
            timer = 0;
        }
        current_queue += 1;
        if (current_queue >= 3)
        {
            current_queue = current_queue % 3;
        }

        if (queues[current_queue].executed == true)
        {
            cout << "Queue " << (current_queue + 1) << " completed\n";
            current_queue += 1;
            if (current_queue >= 3)
            {
                current_queue = current_queue % 3;
            }
            continue;
        }

        if (current_queue == 0)
        {
            cout << "Queue " << (current_queue + 1) << " in hand\n";
            if (rr_timer == 0)
            {
                rr_timer = 4;
            }

            for (int i = 0; i < queues[current_queue].length; i++)
            {
                if (queues[current_queue].processes[i].remaining_time == 0)
                {
                    counter++;
                    continue;
                }

                if (counter == queues[current_queue].length)
                {
                    break;
                }

                while (rr_timer > 0 && queues[current_queue].processes[i].remaining_time != 0 && timer != 10)
                {
                    cout << "Executing queue 1 and P" << (i + 1) << " for a unit time. Process has priority of " << queues[current_queue].processes[i].priority << "\n";
                    queues[current_queue].processes[i].remaining_time--;
                    updateTotalTime(queues);
                    rr_timer--;
                    timer++;
                }

                if (timer == 10)
                {
                    break;
                }

                if (queues[current_queue].processes[i].remaining_time == 0 && rr_timer == 0)
                {
                    rr_timer = 4;
                    if (i == (queues[current_queue].length - 1))
                    {
                        i = -1;
                    }
                    continue;
                }

                if (queues[current_queue].processes[i].remaining_time == 0 && rr_timer > 0)
                {
                    if (i == (queues[current_queue].length - 1))
                    {
                        i = -1;
                    }
                    continue;
                }

                if (rr_timer <= 0)
                {
                    rr_timer = 4;
                    if (i == (queues[current_queue].length - 1))
                    {
                        i = -1;
                    }
                    continue;
                }
            }
        }
        else if (current_queue == 1)
        {
            cout << "Queue " << (current_queue + 1) << " in hand\n";
            sortProcessesByPriority(queues[current_queue]);

            for (int i = 0; i < queues[current_queue].length; i++)
            {
                if (queues[current_queue].processes[i].remaining_time == 0)
                {
                    counter_ps++;
                    continue;
                }

                if (counter_ps == queues[current_queue].length)
                {
                    break;
                }

                while (queues[current_queue].processes[i].remaining_time != 0 && timer != 10)
                {
                    cout << "Executing queue 2 and P" << (i + 1) << " for a unit time. Process has priority of " << queues[current_queue].processes[i].priority << "\n";
                    queues[current_queue].processes[i].remaining_time--;
                    updateTotalTime(queues);
                    timer++;
                }

                if (timer == 10)
                {
                    break;
                }

                if (queues[current_queue].processes[i].remaining_time == 0)
                {
                    continue;
                }
            }
        }
        else
        {
            cout << "Queue " << (current_queue + 1) << " in hand\n";

            for (int i = 0; i < queues[current_queue].length; i++)
            {
                if (queues[current_queue].processes[i].remaining_time == 0)
                {
                    counter_fcfs++;
                    continue;
                }

                if (counter_fcfs == queues[current_queue].length)
                {
                    break;
                }

                while (queues[current_queue].processes[i].remaining_time != 0 && timer != 10)
                {
                    cout << "Executing queue 3 and P" << (i + 1) << " for a unit time. Process has priority of " << queues[current_queue].processes[i].priority << "\n";
                    queues[current_queue].processes[i].remaining_time--;
                    updateTotalTime(queues);
                    timer++;
                }

                if (timer == 10)
                {
                    break;
                }

                if (queues[current_queue].processes[i].remaining_time == 0)
                {
                    continue;
                }
            }
        }
        cout << "Broke from Queue " << (current_queue + 1) << "\n";
        break;
    }

    int total_turnaround_time = 0;
    int total_waiting_time = 0;

    cout << "\nProcess     | Turn Around Time | Waiting Time\n";
    for (int i = 0; i < 3; i++)
    {
        cout << "Queue " << (i + 1) << "\n";
        for (int j = 0; j < queues[i].length; j++)
        {
            cout << "Process P" << (j + 1) << "\t" << queues[i].processes[j].turnaround_time << "\t\t    " << queues[i].processes[j].turnaround_time - queues[i].processes[j].burst_time << "\n";
            total_turnaround_time += queues[i].processes[j].turnaround_time;
            total_waiting_time += queues[i].processes[j].turnaround_time - queues[i].processes[j].burst_time;
        }
    }

    cout << "\nAverage Turnaround Time: " << total_turnaround_time / num_processes << endl;
    cout << "Average Waiting Time: " << total_waiting_time / num_processes << endl;

    return 0;
}
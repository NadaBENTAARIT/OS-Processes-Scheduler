
import matplotlib.pyplot as plt
import  numpy as np


df = open("analysis/data/scheduler_dataset.csv","r")
lines = df.readlines()
#for evey execution:
starts = []
ends = []
durations=[]
jobs = []
for l in lines[1:]:
	ex_id = l.split(',')[0]
	
	process_name = l.split(',')[1]
	arrival_time = l.split(',')[2]
	duraton = l.split(',')[3]
	
	entry_cpu=eval('['+(l.split('[')[1]).split(']')[0]+']')
	
	starts.append(entry_cpu)
		
	exit_cpu = eval('['+(l.split('[')[2]).split(']')[0]+']')
	#for e in exit_cpu:
	ends.append(exit_cpu)
	jobs.append(process_name)
	
	#for e in exit_cpu:
	#	jobs.append(process_name)
durations = []	
for i,e in enumerate(ends) :
	d_process = []
	for j,c in enumerate(ends[i]):
	
		d_process.append( ends[i][j] - starts[i][j] )
	durations.append(d_process)
# print(starts)
# print(ends)
# print(durations)
# print(jobs)


starts_max= [max(s) for s in starts]
durations_max=[max(d) for d in durations]
max_x = starts_max[0]+durations_max[0]
# print(max_x)


# Declaring a figure "gnt"
fig, gnt = plt.subplots()
 
# Setting Y-axis limits
gnt.set_ylim(0, 100)
 
# Setting X-axis limits
gnt.set_xlim(0, max_x)
 
# Setting labels for x-axis and y-axis
gnt.set_xlabel('seconds in CPU')
gnt.set_ylabel('Processor')


# Setting ticks on y-axis
y_ticks=[5]
for i,j in enumerate(jobs[1:]):
	y_ticks.append(y_ticks[i]+6)
gnt.set_yticks(y_ticks)

# Labelling tickes of y-axis
yticklabels = []
for i,j in enumerate(jobs):
	yticklabels.append(str(j))
gnt.set_yticklabels(yticklabels)
 
# Setting graph attribute
gnt.grid(True)
 
# Declaring a bar in schedule
start = 5
for i,job in enumerate(jobs):
	
	periods=[]
	for j,d in enumerate(durations[i]):
		periods.append((starts[i][j],durations[i][j]))
	gnt.broken_barh(periods,(start,5) ,facecolors =('tab:orange'))
	start = start + 6

plt.show()
#plt.savefig("gantt1.png")

import matplotlib.pyplot as plt
import csv
import numpy as np

plt.ioff()
fig = plt.figure(figsize=(12,8), dpi=600)
plt.axis([0,0.3,0,46000])
plt.title('Motor startup with various brushed and brushless motors with modified Simonk firmware')
plt.xlabel('Seconds since startup command')
plt.ylabel('RPM')

files = ['flywheel-700-6zc-0_5-1','inrunner-XK2445-3500','inrunner-XK2858-2700','inrunner-B2857-15L-1900','inrunner-XK2845-1900','inrunner-XK2845-B-2000','outrunner-NTM-3530-1400']
labels = ['Outrunner 2627-3800 19A @ 3S','Inrunner XK2445-3500 8A @ 3S','Inrunner XK2858-2700 11A @ 3S','Inrunner B2857-15L-1900 7A @ 3S','Inrunner XK2845-1900 5A @ 3S','Inrunner XK2845-B-2000 5A @ 3S','Outrunner NTM 3530-1400 16A @ 3S','MTB Wolverine 47A @ 2S','3240 17A @ 3S','MTB Hellcat 22A @ 3S','3240 11A @ 2S','MTB Honeybadger 14A @ 2S','MTB Rhino 7A @ 3S']
colors = ['blue','red','green','orange','purple','yellow','teal','#7E2F8E','#B5455B','#ECB01F','#4DBEEE','#D85218','#0072BD']
multipliers = [1, 3/2, 3/2, 3, 3/2, 3/2, 3/4]

i = 0

brushed = True
regressions = False

for fileName in files:
    file = open(files[i]+'.csv')
    reader = csv.reader(file)
    times = []
    speeds = []
    for row in reader:
        if len(row) == 2:
            times.append(float(row[0]))
            speeds.append(float(row[1])*multipliers[i])
        elif len(row) != 0:
            print('Error:')
            print(row)
    plt.scatter(times, speeds, color=colors[i], s=0.5, label=labels[i])
    if regressions:
        xp = np.linspace(0,0.5,500)
        p = np.poly1d(np.polyfit(np.array(times), np.array(speeds), 5))
        plt.plot(xp, p(xp), color=colors[i], label=labels[i], linewidth=2)
    file.close()
    i+=1

if brushed:
    for fileName in ['brushed-MTB Wolverine','brushed-3240 @ 3S','brushed-MTB Hellcat','brushed-3240 @ 2S','brushed-MTB Honeybadger','brushed-MTB Rhino']:
        file = open(fileName+'.csv')
        reader = csv.reader(file)        
        times = []
        speeds = []
        for row in reader:
            times.append(float(row[0]))
            speeds.append(float(row[1])*1000)
        plt.plot(times, speeds, label=labels[i], color=colors[i], linewidth=3)
        file.close()
        i+=1

plt.legend(loc='lower right', scatterpoints=1, markerscale=10)
fig.savefig('simonk-capture-'+'-'.join(files)+('-brushed' if brushed else '')+('-regressions' if regressions else '')+'.png', bbox_inches='tight')
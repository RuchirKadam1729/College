import json
import pandas as pd
from statistics import mean

'''
5. What is the average load time of the webpage without any network throttling?
6. After applying the "Slow 3G" network profile, how does the page load time change compared to the normal network?
7. Which resources (e.g., images, scripts, CSS files) take the longest to load when the page is throttled with "Regular 3G"?
8. Identify any 404 or other errors in the network log when loading the page with "Slow 3G"
applied. What might cause these errors?
9. Does the page load faster or slower with "Fast 3G" compared to "Slow 3G"? What are
the reasons for the differences?
10. What is the "Time to First Byte (TTFB)" for the webpage under different throttling
profiles? How does network throttling affect TTFB?
11. How does the "DOMContentLoaded" time change when testing under different throttling
profiles?
12. What is the impact of throttling on the "Load" event timing (when the page is fully
loaded)?
13. Is there any noticeable delay in rendering when the page is loaded under "Slow 3G"
conditions? If so, what resources are responsible for the delay?
14. Are there any large resources (e.g., images, scripts) that cause the page to load slowly
under network throttling? Identify them.
'''

def pages(harFile):
    har = json.load(open(harFile))
    return har['log']['pages']

def entries(harFile):
    har = json.load(open(harFile))
    return har['log']['entries']


def avgLoadTime(harFile):
    return mean([page['pageTimings']['onLoad'] for page in pages(harFile)])

def loadTimePerMimeType(harFile):
    listOfDicts = [{entry['response']['content']['mimeType']:entry['timings'].get('wait',0)} for entry in entries(harFile)]
    return pd.DataFrame(listOfDicts).aggregate('mean')

def check404(harfile):
    return sum([entry['response']['status']==404 for entry in entries(harfile)])

def avgTTFB(harFile):
    return mean([entry['timings'].get('wait',0) for entry in entries(harFile)])

def avgDOMLoadTime(harFile):
    return mean([page['pageTimings']['onContentLoad'] for page in pages(harFile)])

def avgRenderTime(harFile) : 
    return avgLoadTime(harFile) - avgDOMLoadTime(harFile)

def sizePerMimeType(harFile):
    listOfDicts = [\
        {entry['response']['content']['mimeType']: \
            entry['response']['content']['size']} \
        for entry in entries(harFile)]
    
    return (
    pd.DataFrame(listOfDicts)               
    .replace(0, pd.NA).aggregate('mean')
    )

def evaluateAllTimings(harFile):
    entries1 = entries(harFile)
    def helper1(access):
        return mean([entry['timings'].get(access, 0) for entry in entries1]) 
    
    list1 = ['blocked', 'dns', 'connect', 'ssl', 'send', 'wait', 'receive']
    col1 = ['onContentLoad', 'onLoad']+list1
    col2 = [avgLoadTime(harFile),avgDOMLoadTime(harFile)]+list(map(lambda x:helper1(x),list1))
    print(pd.DataFrame({'timing':col1,'mean val':col2}))

funcs = [avgLoadTime,loadTimePerMimeType,check404,avgTTFB,avgDOMLoadTime,avgRenderTime,sizePerMimeType]

processed = [func('har/desmos.har') for func in funcs]

#print(processed)

print(evaluateAllTimings('har/desmos.har'))
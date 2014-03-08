import scipy.stats as stats

features = [30.0,30.0,30.0,38.0,40.0,29.0,29.0,29.0,40.0,41.0,43.0,39.0,120.0,120.0,119.0]
kernel = stats.gaussian_kde(features)
for i in range(0,1100):
        #print '%i\t%f\n'%(i,kernel.evaluate(i))
        if(kernel.evaluate(i) > kernel.evaluate(i-1) and kernel.evaluate(i) > kernel.evaluate(i+1)):
                print "Found a max at %i",i
        #print "%i\t%f"%(i,kernel.evaluate(i))


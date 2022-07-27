import numpy as np

def schr(p,v,dx,hbar,m):
  dp = [0] * len(p)
  dp = np.array(dp, dtype=complex)
  if len(p) > 2 and len(v) > 1:
    dp[1] = 1j*hbar/(2*m)*(p[0]-2*p[1]+p[2])/(dx**2) - 1j*v[1]*p[1]/hbar
    dp[len(p)-2] = 1j*hbar/(2*m)*(p[len(p)-1]-2*p[len(p)-2]+p[len(p)-3])/(dx**2) - 1j*v[len(p)-2]*p[len(p)-2]/hbar
    for i in range(2,len(p)-2):
      dp[i] = 1j*hbar/(2*m)*(-p[i+2]+16*p[i+1]-30*p[i]+16*p[i-1]-p[i-2])/(12*dx**2) - 1j*v[i]*p[i]/hbar
  return dp

v0 = 0.3
dx = 0.05
dt = 0.01
hbar = 1
m = 125
a = 10
x = np.arange(-2 * a, 2 * a + dx, dx)
c = 10
p = (2/np.pi)**(1/4)*np.multiply(np.exp(-np.power(x+a/2,2)), np.exp(1j*c*x))
r = hbar*c/(2*m)*dt/dx

v = [0] * len(x)
for i in range(len(x)):
  if abs(x[i]) <= a/5 and abs(x[i]) >= a/15:
    v[i] = v0
v = np.array(v)

k1 = schr(p,v,dx,hbar,m)
k2 = schr(p+dt*np.divide(k1,2),v,dx,hbar,m)
k3 = schr(p+dt*np.divide(k2,2),v,dx,hbar,m)
k4 = schr(p+np.multiply(dt,k3),v,dx,hbar,m)
a = p[len(p)-1]
b = p[len(p)-2]
c = p[0]
d = p[1]
k = 6 * (k1 + 2*k2 + 2*k3 +k4)
p = p + np.divide(dt, k, out=np.zeros_like(k), where=k!=0)
#p = p + dt / 6 * (k1 + 2*k2 + 2*k3 +k4)
#p = p + np.divide(dt, 6 * (k1 + 2*k2 + 2*k3 +k4))
p[len(p)-1] = b+(r-1)/(r+1)*(p[len(p)-2]-a)
p[0] = d+(r-1)/(r+1)*(p[1]-c)

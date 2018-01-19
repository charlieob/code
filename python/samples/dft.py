# http://jakevdp.github.io/blog/2013/08/28/understanding-the-fft/

import numpy as np

# Euler's identity: e^(i*pi)+1=0
print "e^(i*pi)+1 = ", np.exp(1j*np.pi)+1
print "Euler's identity:", np.allclose(np.exp(1j*np.pi)+1, np.array(0.0))

def DFT_slow(x):
    """Compute the discrete Fourier Transform of the 1D array x"""
    # x = np.asarray(x, dtype=float)
    x = np.asarray(x)       # dropping the dtype specifier in order to use this function in calculating the inverse DFT
    N = x.shape[0]
    n = np.arange(N)
    k = n.reshape((N, 1))
    M = np.exp(-2j * np.pi * k * n / N)
# n is 1D 1024, k is 2D 1024x1
# ndarray element-wise multiplication with broadcasting, k*n yields 1024x1024
    return np.dot(M, x)
# matrix multiplication (dot product): (1024x1024) x (1024x1) yields (1024x1)
# a 1D array in ndarray terms, with shape=(1024,)

# For example:
# >>> n=np.arange(4)
# >>> n
# array([0, 1, 2, 3])
# >>> k = n.reshape((4,1))
# >>> k
# array([[0],
       # [1],
       # [2],
       # [3]])
# >>> k*n
# array([[0, 0, 0, 0],
       # [0, 1, 2, 3],
       # [0, 2, 4, 6],
       # [0, 3, 6, 9]])
# >>> n*k
# array([[0, 0, 0, 0],
       # [0, 1, 2, 3],
       # [0, 2, 4, 6],
       # [0, 3, 6, 9]])
#

def IDFT_slow(x):
    """Compute the inverse DFT of the 1D array x"""
    x = np.asarray(x)
    N = x.shape[0]
    n = np.arange(N)
    k = n.reshape((N, 1))
    M = np.exp(2j * np.pi * k * n / N)      # (1) minus sign in exponent removed
    return np.dot(M, x)/N                   # (2) scaling result by 1/N

x = np.random.random(1024)
print "DFT_slow vs numpy.fft:", np.allclose(DFT_slow(x), np.fft.fft(x))

print "inverse FFT:", np.allclose(np.fft.ifft(np.fft.fft(x)), x)
print "IDFT_slow:", np.allclose(IDFT_slow(DFT_slow(x)), x)

# reverse DFT using the forward DFT procedure
dft = DFT_slow(x)
dft[1:] = dft[:0:-1]    # to compute the reverse DFT first reverse all the inputs except index=0
print "reverse DFT_slow:", np.allclose(DFT_slow(dft)/1024, x)

import time

start_time = time.clock()
for i in range(10): DFT_slow(x)
print("DFT (10): " + str(time.clock() - start_time))

def FFT(x):
    """A recursive implementation of the 1D Cooley-Tukey FFT"""
    x = np.asarray(x, dtype=float)
    N = x.shape[0]
    
    if N % 2 > 0:
        raise ValueError("size of x must be a power of 2")
    elif N <= 32:  # this cutoff should be optimized
        return DFT_slow(x)
    else:
        X_even = FFT(x[::2])    # s[i:j:k] is slice of s from i to j with step k
        X_odd = FFT(x[1::2])    # the odd-indexed elements
        factor = np.exp(-2j * np.pi * np.arange(N) / N)
        # X_even and X_odd are Fourier transforms of length N/2, periodic in N/2
        # hence X_even[N/2] = X_even[0],
        #       X_even[N/2+1] = X_even[1], etc
        return np.concatenate([X_even + factor[:N / 2] * X_odd,
                               X_even + factor[N / 2:] * X_odd])

print "FFT vs numpy.fft:", np.allclose(FFT(x), np.fft.fft(x))

start_time = time.clock()
for i in range(10): FFT(x)
print("FFT (10): " + str(time.clock() - start_time))

def FFT2(x):
    """A recursive implementation of the 1D Cooley-Tukey FFT
       Minor rewrite of FFT() above to match more closely with description in
            https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm
    """
    x = np.asarray(x, dtype=float)
    N = x.shape[0]
    
    if N % 2 > 0:
        raise ValueError("size of x must be a power of 2")
    elif N <= 32:  # this cutoff should be optimized
        return DFT_slow(x)
    else:
        X_even = FFT(x[::2])
        X_odd = FFT(x[1::2])
        factor = np.exp(-2j * np.pi * np.arange(N/2) / N)
        return np.concatenate([X_even + factor[:] * X_odd,
                               X_even - factor[:] * X_odd])

print "FFT2 vs numpy.fft:", np.allclose(FFT2(x), np.fft.fft(x))

start_time = time.clock()
for i in range(10): FFT2(x)
print("FFT2 (10): " + str(time.clock() - start_time))

def FFT_vectorized(x):
    """A vectorized, non-recursive version of the Cooley-Tukey FFT"""
    x = np.asarray(x, dtype=float)
    N = x.shape[0]

    if np.log2(N) % 1 > 0:
        raise ValueError("size of x must be a power of 2")

    # N_min here is equivalent to the stopping condition above,
    # and should be a power of 2
    N_min = min(N, 32)
    
    # Perform an O[N^2] DFT on all length-N_min sub-problems at once
    n = np.arange(N_min)
    k = n[:, None]      # None is an alias for np.newaxis; k is a 2-d array
    M = np.exp(-2j * np.pi * n * k / N_min)     # M is a 32x32 array
    # x is a 1D array of size N
    # x.reshape() gives a 2D array of size 32 x (N/32)
    X = np.dot(M, x.reshape((N_min, -1)))       # matrix mult: (32x32) x (32 x N/32) --> (32 x N/32)

    # build-up each level of the recursive calculation all at once
    while X.shape[0] < N:
        X_even = X[:, :X.shape[1] / 2]
        X_odd = X[:, X.shape[1] / 2:]
        factor = np.exp(-1j * np.pi * np.arange(X.shape[0])
                        / X.shape[0])[:, None]
        X = np.vstack([X_even + factor * X_odd,
                       X_even - factor * X_odd])

    return X.ravel()

print "FFT_vectorized vs numpy.fft:", np.allclose(FFT_vectorized(x), np.fft.fft(x))

start_time = time.clock()
for i in range(10): FFT_vectorized(x)
print("FFT_v (10): " + str(time.clock() - start_time))

x = np.random.random(1024 * 16)
print "FFT_vectorized vs numpy.fft:", np.allclose(FFT_vectorized(x), np.fft.fft(x))

start_time = time.clock()
for i in range(10): np.fft.fft(x)
print("np.fft (10): " + str(time.clock() - start_time))

start_time = time.clock()
for i in range(10): FFT_vectorized(x)
print("FFT_v (10): " + str(time.clock() - start_time))

/* RunningStatInt - calculates mean, variance and standard deviation 
 *  without storing all values and storing intermediate values as integers
 *  to keep optimal accuracy for CPU's with small variable footprint.
 *  This class is intended for Arduino AVR CPUs and values with limited 
 *  range; like 3D printer filament diameter variation.
 * 
 * The implementation is based on this description/code:
 *  https://www.johndcook.com/blog/standard_deviation/. The conversion
 *  to and from long ints (32-bit on the 8-bit avr cpus!) is mine. As
 *  are the Min()/Max() methods.
 * 
 * fjkraan@electrickery.nl, 2021-08-23
 */

// an indicaton of the number of fractional digits preserved in the long int. 
#define D2LFACTOR 10000.0 
//#include <cstdio>
#include <math.h>
//#include "RunningStatInt.h"

class RunningStatInt
    {
    public:
        RunningStatInt() : m_n(0) {
            d2lfactorSquared = D2LFACTOR * D2LFACTOR;
        }

        void Clear()
        {
            m_n = 0;
            min = 0;
            max = 0;
        }

        void Push(double x)
        {
            m_n++;
            long xFactored = (long)(x * D2LFACTOR);

            // See Knuth TAOCP vol 2, 3rd edition, page 232
            if (m_n == 1)
            {
                m_oldM = m_newM = xFactored;
                min = max = x;
            }
            else
            {
                m_newM = m_oldM + (xFactored - m_oldM) / m_n;
                m_newS = m_oldS + (xFactored - m_oldM) * (xFactored - m_newM);
   
//            printf(" m_oldM: %ld, m_newM: %ld", m_oldM, m_newM);
//            printf(" m_oldS: %ld, m_newS: %ld", m_oldS, m_newS);
//            printf("\n");
                // set up for next iteration
                m_oldM = m_newM; 
                m_oldS = m_newS;
                
                // extra boundary values
                min = (x < min) ? x : min;
                max = (x > max) ? x : max;
            }
        }

        int NumDataValues() const
        {
            return m_n;
        }

        double Mean() const
        {
            return (m_n > 0) ? ((double)m_newM) / D2LFACTOR : 0.0;
        }

        double Variance() const
        {
            // return ( (m_n > 1) ? m_newS/(m_n - 1) : 0.0 );
            double m_newsS_Double = ((double)m_newS) / d2lfactorSquared;
            return ( (m_n > 1) ? m_newsS_Double / (m_n - 1) : 0.0 );
        }

        double StandardDeviation() const
        {
            return sqrt( Variance() );
        }
        
        double Min() const
        {
            return min;
        }
        
        double Max() const
        {
            return max;
        }

    private:
        int m_n;
        // m_...S is 'squared' domain
        long int m_oldM, m_newM, m_oldS, m_newS;
        double min, max, d2lfactorSquared;
    };

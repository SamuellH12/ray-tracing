#ifndef BLINEHEADER
#define BLINEHEADER
#include <iostream>
#include <math.h>
#include "../Point.cpp"
#include "../Vector.cpp"
#include "../Ray.cpp"
#include "Objeto.cpp"
#include <vector>
#include <tuple>

std::vector<std::vector<long long>> BINOMIOS (1, {1});


void _calcBinomio(int N){
    if(!BINOMIOS[N].empty()) return;
    if(BINOMIOS.size() <= N)  BINOMIOS.resize(N+1);
    _calcBinomio(N-1);

    BINOMIOS[N].resize(N+1);
    BINOMIOS[N][0] = BINOMIOS[N][N] = 1;

    //NcK(N, K) = NcK(N-1, K) + NcK(N-1, K-1)
    for(int K=1; K<N; K++) 
        BINOMIOS[N][K] = BINOMIOS[N-1][K] + BINOMIOS[N-1][K-1];
}

long long _NcK(int N, int K){ //N Choose K
    return BINOMIOS[N][K];
}

double Bernstein(int N, int i, double t){ return _NcK(N, i) * pow(t, i) * pow(1.0 - t, N-i); }

class Bline : public objeto {
private:
public:
    std::vector<point> pts;
    double D;
    bool showMeta = false;
    
    point BX1, BX2;

    Bline(){}
    Bline(std::vector<point> pts, Color color, double D=0.125, bool showMeta = false) : objeto(pts.front(), color), pts(pts), D(D), showMeta(showMeta) { 
        _calcBinomio(pts.size()); 
        auto tmp = calc_boundbox(pts);
        BX1 = tmp.first;
        BX2 = tmp.second;
        BX1.print();
        BX2.print();
    }

    point getPt(double t){
        point pt(0, 0, 0);
        for(int N=pts.size()-1, i=0; i<=N; i++)
            pt = pt + pts[i].to_vetor() * Bernstein(N, i, t);
        return pt;
    }

    point getPt(point &a, point &b, double t){
        return point() + a.to_vetor()*(1.0-t) + b.to_vetor()*(t);
    }

    bool has_intersection(ray &r, double tmax = std::numeric_limits<double>::infinity()) override{
        return false;
    }

    Intersection solve(std::vector<point> p, ray &r, int lvl = 7){
        auto [bx1, bx2] = calc_boundbox(p);
        if(!boundbox_intersection(r, bx1, bx2)) return Intersection();

        if(lvl <= 0) // (p.front() - p.back()).norm2() < D*D ||
        {
            point pt = getPt(p.front(), p.back(), 0.5);
            vetor oc = r.get_origin() - pt;
            double a = r.get_direction() * r.get_direction();
            double b = 2.0 * (oc * r.get_direction());
            double c = oc*oc - D*D;
            double dlt = b*b - 4.0*a*c;
            if(dlt < 0.0) return Intersection();

            double raiz = sqrt(dlt);
            double t = (-b - raiz) / (2.0*a);
            if(t < 0) t = (-b + raiz) / (2.0*a);
            if(t < 0) return Intersection();
            vetor normal = (r.get_point(t) - pt).normalized();
            return Intersection(t, normal, ka);
        }

        std::vector<point> L, R;

        while(p.size() > 1)
        {
            L.push_back(p.front());
            R.push_back(p.back());

            for(int i=0; i+1<p.size(); i++)
                p[i] = getPt(p[i], p[i+1], 0.5);
            p.pop_back();
        }

        L.push_back(p.front());
        R.push_back(p.back());
        for(int i=0, j=R.size()-1; i<j; i++, j--) std::swap(R[i], R[j]);

        return std::min(solve(L, r, lvl-1), solve(R, r, lvl-1));
    }

    Intersection get_intersection(ray &r, Luz const &Ia, std::vector<Luz> const &luzes, std::vector<objeto*> const &objetos) override{ 
        // if(!boundbox_intersection(r, BX1, BX2)) return Intersection();
        Intersection ans = solve(pts, r);
        if(ans.normal.norm2() > 0) ans.color = get_color(r, r.get_point(ans.dist), ans.normal, Ia, luzes, objetos);
        return ans;
    }

private:
    std::pair<point, point> calc_boundbox(std::vector<point> &pt){
        double xn, yn, zn, xm, ym, zm;
        xn = yn = zn = std::numeric_limits<double>::infinity();
        xm = ym = zm = -xn;

        for(auto& p : pt){
            xn = std::min(xn, p.getX());
            yn = std::min(yn, p.getY());
            zn = std::min(zn, p.getZ());
            
            xm = std::max(xm, p.getX());
            ym = std::max(ym, p.getY());
            zm = std::max(zm, p.getZ());
        }

        return {point(xn-D, yn-D, zn-D), point(xm+D, ym+D, zm+D)};
    }
};
#endif
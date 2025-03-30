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
int BezierRecLvl = 7;

class Bline : public objeto {
private:
public:
    std::vector<point> pts;
    double D;
    bool showMeta = false;
    double metat = 0.5;
    double METASZ = 3;
    
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

    double get_dist(std::vector<point> p, ray &r, int lvl = BezierRecLvl){
        auto [bx1, bx2] = calc_boundbox(p);
        if(!boundbox_intersection(r, bx1, bx2)) return DOUBLEINF;

        if(lvl <= 0) // (p.front() - p.back()).norm2() < D*D ||
        {
            point pt = getPt(p.front(), p.back(), 0.5);
            vetor oc = r.get_origin() - pt;
            double a = r.get_direction() * r.get_direction();
            double b = 2.0 * (oc * r.get_direction());
            double c = oc*oc - D*D;
            double dlt = b*b - 4.0*a*c;
            if(dlt < 0.0) return DOUBLEINF;

            double raiz = sqrt(dlt);
            double t = (-b - raiz) / (2.0*a);
            if(t < 0) t = (-b + raiz) / (2.0*a);
            if(t < 0) return DOUBLEINF;
            return t;
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

        return std::min(get_dist(L, r, lvl-1), get_dist(R, r, lvl-1));
    }

    double dist_intersection(ray &r) override{
        if(showMeta) return std::min(get_dist(pts, r), metaInter(r, pts, METASZ).dist);
        return get_dist(pts, r);
    }

    Intersection solve(std::vector<point> p, ray &r, int lvl = BezierRecLvl){
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
            return Intersection(t, normal, Color(0, 1, 0));
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

    Intersection get_intersection(ray &r, Luz const &Ia, std::vector<Luz> const &luzes, std::vector<objeto*> const &objetos, int profundidade = MAXREC) override{ 
        // if(!boundbox_intersection(r, BX1, BX2)) return Intersection();
        Intersection ans = solve(pts, r);

        // if(ans.normal.norm2() > 0) ans.color = get_color(r, r.get_point(ans.dist), ans.normal, Ia, luzes, objetos);

        if(showMeta) ans = std::min(ans, metaInter(r, pts, METASZ)); 

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

    Intersection metaInter(ray &r, std::vector<point> pt, double h=0.5)
    {
        if(pt.empty()) return Intersection();
        Intersection ans = Intersection();

        for(int i=0; i+1<pt.size(); i++)
            ans = std::min(ans, std::min(point_intersection(r, pt[i], h), line_intersection(r, pt[i], pt[i+1], h*0.30)));

        ans = std::min(ans, point_intersection(r, pt.back(), pt.size() == 1 ? METASZ : h));

        for(int i=0; i+1<pt.size(); i++)
                pt[i] = getPt(pt[i], pt[i+1], metat);
        
        pt.pop_back();

        ans = std::min(ans, metaInter(r, pt, h*0.75));

        return ans;
    }

    Intersection point_intersection(ray &r, point p, double h){
        vetor oc = r.get_origin() - p;
        double a = r.get_direction() * r.get_direction();
        double b = 2.0 * (oc * r.get_direction());
        double c = oc * oc - h*h;
        double dlt = b*b - 4.0*a*c;

        if(dlt < 0.0) return Intersection();

        double raiz = sqrt(dlt);
        double t = (-b - raiz) / (2.0*a);
        if(t < 0) t = (-b + raiz) / (2.0*a);
        if(t < 0) return Intersection();

        point inter = r.get_origin() + (r.get_direction()*t);  
        vetor normal = (inter - p).normalized();

        return Intersection(t, normal, Color(1, 0, 0));
    }

    Intersection line_intersection(ray &r, point p, point q, double h){
        vetor v = q-p;
        vetor oc = r.get_origin() - p;
        vetor dir = r.get_direction();
        vetor axis_norm = v.normalized();

        double a = (dir - axis_norm * (dir * axis_norm)).norm2();
        double b = 2.0 * ((dir - axis_norm * (dir * axis_norm)) * (oc - axis_norm * (oc * axis_norm)));
        double c = (oc - axis_norm * (oc * axis_norm)).norm2() - h*h;
        double dlt = b*b - 4.0*a*c;

        if (dlt < 0.0) return Intersection();

        double raiz = sqrt(dlt);
        double t = (-b - raiz) / (2.0 * a);
        bool changed = false;
        if (t < 0) t = (-b + raiz) / (2.0 * a), changed = true;
        if (t < 0) return Intersection();

        point inter = r.get_origin() + r.get_direction() * t;
        double projection = (inter - p) * axis_norm;
        
        if(!changed && (projection < 0 || projection*projection > v.norm2())) // se falhou, testa com o outro
        { 
            t = (-b + raiz) / (2.0 * a);
            inter = r.get_origin() + r.get_direction() * t;
            projection = (inter - p) * axis_norm;
        }

        if (projection < 0 || projection*projection > v.norm2()) return Intersection();

        vetor normal = inter - (p + axis_norm * projection);
        double seno = (normal%r.get_direction()).norm() / (normal.norm() * (r.get_direction().norm()));

        return Intersection(t, normal, Color(0, 0, 1));
    }
};
#endif
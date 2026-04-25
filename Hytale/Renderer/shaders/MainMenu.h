/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <string>

static const std::string CITY_FRAG = R"(#version 330 core

in vec2 v_TexCoord;
in vec2 v_OneTexel;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform vec2 u_Size;
uniform float u_Time;

/*
    Tetromino Shaded Isometric City
    ------------------------
    (C) Koding Nights
*/

#define BOARD_W 5
#define BOARD_H 5
#define BOARD_CELLS 25

float hash21(vec2 p){
    return fract(sin(dot(p, vec2(27.619, 57.583))) * 43758.5453);
}

vec2 iso(vec2 p, float z){
    return vec2(p.x - p.y, (p.x + p.y) * 0.5 - z);
}

vec2 isoInv(vec2 p){
    return vec2(p.y + p.x * 0.5, p.y - p.x * 0.5);
}

vec3 districtColor(float idx){
    if(idx < 0.5) return vec3(0.41, 0.72, 1.00); // CIVIC
    if(idx < 1.5) return vec3(1.00, 0.59, 0.48); // COMMERCIAL
    if(idx < 2.5) return vec3(0.76, 0.60, 1.00); // INDUSTRIAL
    if(idx < 3.5) return vec3(1.00, 0.89, 0.42); // RESIDENTIAL
    if(idx < 4.5) return vec3(0.91, 0.87, 0.80); // MONUMENT
    return vec3(0.70, 0.91, 0.44); // PARK
}

vec3 pieceColor(vec2 lotId, int piece){
    float fp = float(piece);
    float rnd = floor(hash21(lotId + vec2(fp * 2.13 + 1.9, fp * 1.37 + 4.1)) * 6.0);
    return districtColor(rnd);
}

float faceMask(vec2 q, vec2 sz, float aa, out float edgeDist){
    edgeDist = min(min(q.x, q.y), min(sz.x - q.x, sz.y - q.y));
    return smoothstep(0.0, aa, edgeDist);
}

float topRectMask(vec2 p, vec2 worldCorner, float h, vec2 size, float aa, out float edgeDist){
    vec2 q = isoInv(p - iso(worldCorner, h));
    return faceMask(q, size, aa, edgeDist);
}

float topMask(vec2 p, vec2 worldCorner, float h, float size, float aa, out float edgeDist){
    return topRectMask(p, worldCorner, h, vec2(size), aa, edgeDist);
}

float leftMask(vec2 p, vec2 worldCorner, float h, float size, float aa, out float edgeDist){
    vec2 r = p - iso(worldCorner + vec2(0.0, size), h);
    vec2 q = vec2(r.x, r.y - r.x * 0.5);
    return faceMask(q, vec2(size, h), aa, edgeDist);
}

float rightMask(vec2 p, vec2 worldCorner, float h, float size, float aa, out float edgeDist){
    vec2 r = p - iso(worldCorner + vec2(size, 0.0), h);
    vec2 q = vec2(-r.x, r.y + r.x * 0.5);
    return faceMask(q, vec2(size, h), aa, edgeDist);
}

float lotFilled(vec2 id){
    return 1.0;
}

float lotHeight(vec2 id, float tile){
    return tile;
}

int lotLayoutId(vec2 id){
    float rnd = hash21(id + 1.7);
    if(rnd < 0.28) return 0;
    if(rnd < 0.56) return 1;
    if(rnd < 0.84) return 2;
    return 3;
}

vec2 lotBoardOrigin(vec2 lotCenter, float tile){
    return lotCenter - vec2(float(BOARD_W), float(BOARD_H)) * tile * 0.5;
}

/*
    Layout encoding:
    5x5 board, bit index = y * 5 + x, with x/y in unflipped layout space.

    layout 0:
    y0: 11100
    y1: 11101
    y2: 00001
    y3: 00011
    y4: 00001

    layout 1:
    y0: 11100
    y1: 11100
    y2: 00010
    y3: 11011
    y4: 11000

    layout 2:
    y0: 11101
    y1: 11101
    y2: 00001
    y3: 11000
    y4: 11010

    layout 3:
    y0: 11010
    y1: 11011
    y2: 00111
    y3: 11001
    y4: 11001
*/

const int LAYOUT0_OCC = 9111;
const int LAYOUT1_OCC = 14271;
const int LAYOUT2_OCC = 9695;
const int LAYOUT3_OCC = 20331;

int layoutOccMask(int layoutId){
    if(layoutId == 0) return LAYOUT0_OCC;
    if(layoutId == 1) return LAYOUT1_OCC;
    if(layoutId == 2) return LAYOUT2_OCC;
    return LAYOUT3_OCC;
}

int boardToLayoutIndex(int x, int y, int flipX, int flipY){
    if(x < 0 || y < 0 || x >= BOARD_W || y >= BOARD_H) return -1;
    if(flipX == 1) x = BOARD_W - 1 - x;
    if(flipY == 1) y = BOARD_H - 1 - y;
    return y * BOARD_W + x;
}

bool occAt(int occMask, int x, int y, int flipX, int flipY){
    int idx = boardToLayoutIndex(x, y, flipX, flipY);
    if(idx < 0) return false;
    return ((occMask >> idx) & 1) != 0;
}

/*
    Piece id is still determined analytically from layout-space cell coordinates.
    This preserves original coloring/grouping without using the old giant branch tree
    everywhere in the hot path.
*/
int pieceAtLayoutCell(int layoutId, int x, int y){
    if(x < 0 || y < 0 || x >= BOARD_W || y >= BOARD_H) return -1;

    if(layoutId == 0){
        if(y == 0 || y == 1){
            if(x < 3) return 0;
            if(y == 1 && x == 4) return 1;
            return -1;
        }
        if(y == 2){
            if(x == 4) return 1;
            return -1;
        }
        if(y == 3){
            if(x > 2) return 1;
            return -1;
        }
        if(x == 4) return 1;
        return -1;
    }

    if(layoutId == 1){
        if(y == 0 || y == 1){
            if(x < 3) return 0;
            return -1;
        }
        if(y == 2){
            if(x == 3) return 2;
            return -1;
        }
        if(y == 3){
            if(x < 2) return 1;
            if(x > 1) return 2;
            return -1;
        }
        if(x < 2) return 1;
        return -1;
    }

    if(layoutId == 2){
        if(y == 0 || y == 1){
            if(x < 3) return 0;
            if(x == 4) return 2;
            return -1;
        }
        if(y == 2){
            if(x == 4) return 2;
            return -1;
        }
        if(y == 3 || y == 4){
            if(x < 2) return 1;
            if(y == 4 && x == 3) return 3;
            return -1;
        }
        return -1;
    }

    if(y == 0){
        if(x < 2) return 0;
        if(x == 3) return 2;
        return -1;
    }
    if(y == 1){
        if(x < 2) return 0;
        if(x > 2) return 2;
        return -1;
    }
    if(y == 2){
        if(x == 2) return 4;
        if(x > 2) return 2;
        return -1;
    }
    if(y == 3){
        if(x < 2) return 1;
        if(x == 4) return 3;
        return -1;
    }
    if(x < 2) return 1;
    if(x == 4) return 3;
    return -1;
}

int pieceAtBoardCell(int layoutId, int x, int y, int flipX, int flipY){
    if(x < 0 || y < 0 || x >= BOARD_W || y >= BOARD_H) return -1;
    if(flipX == 1) x = BOARD_W - 1 - x;
    if(flipY == 1) y = BOARD_H - 1 - y;
    return pieceAtLayoutCell(layoutId, x, y);
}

vec3 renderGround(vec2 w, float pitch, float plotSize, float aa){
    vec2 id = floor(w / pitch + 0.5);
    vec2 q = w - id * pitch;

    vec2 aq = abs(q);
    float qmax = max(aq.x, aq.y);
    float halfPlot = plotSize * 0.5;
    float boxDist = qmax - halfPlot;
    float inner = halfPlot - qmax;
    float m = 1.0 - smoothstep(0.0, aa * 1.6, boxDist);

    vec3 road = vec3(0.40, 0.39, 0.36);
    float tone = hash21(id + 9.7);
    vec3 grass = mix(vec3(0.18, 0.63, 0.29), vec3(0.23, 0.78, 0.38), tone);
    grass *= 0.98 + 0.02 * sin((id.x + id.y) * 1.7);

    vec3 col = mix(road, grass, m);
    float rim = m * (1.0 - smoothstep(aa * 1.2, aa * 3.5, inner));
    col = mix(col, vec3(0.96, 0.93, 0.72), rim * 0.9);
    return col;
}

struct LotInfo {
    vec2 id;
    vec2 center;
    vec2 origin;
    int layoutId;
    int flipX;
    int flipY;
    int occMask;
};

LotInfo makeLotInfo(vec2 id, float pitch, float tile){
    LotInfo L;
    L.id = id;
    L.center = id * pitch;
    L.origin = lotBoardOrigin(L.center, tile);
    L.layoutId = lotLayoutId(id);
    L.flipX = int(floor(hash21(id + 2.8) * 2.0));
    L.flipY = int(floor(hash21(id + 3.6) * 2.0));
    L.occMask = layoutOccMask(L.layoutId);
    return L;
}

bool pointCouldTouchLotOrShadow(vec2 w, vec2 origin, float tile){
    vec2 maxP = origin + vec2(float(BOARD_W) + 1.0, float(BOARD_H)) * tile;
    return all(greaterThanEqual(w, origin)) && all(lessThan(w, maxP));
}

float groundShadowAt(vec2 w, vec2 baseId, float pitch, float tile, float aa){
    float shadow = 0.0;
    bool occupied = false;

    for(int j = -1; j <= 1; j++){
        for(int i = -1; i <= 1; i++){
            vec2 id = baseId + vec2(float(i), float(j));
            if(lotFilled(id) < 0.5) continue;

            LotInfo L = makeLotInfo(id, pitch, tile);

            if(!pointCouldTouchLotOrShadow(w, L.origin, tile)) continue;

            vec2 local = (w - L.origin) / tile;

            int fx = int(floor(local.x));
            int fy = int(floor(local.y));

            if(occAt(L.occMask, fx, fy, L.flipX, L.flipY)){
                occupied = true;
            }

            int sx = int(floor(local.x - 1.0));
            int sy = int(floor(local.y));

            if(occAt(L.occMask, sx, sy, L.flipX, L.flipY)){
                vec2 casterCorner = L.origin + vec2(float(sx), float(sy)) * tile;
                vec2 q = w - (casterCorner + vec2(tile, 0.0));

                float edge;
                float m = faceMask(q, vec2(tile, tile), aa, edge);

                float eastFade = 1.0 - smoothstep(tile * 0.60, tile, q.x);
                shadow = max(shadow, m * eastFade);
            }
        }
    }

    return occupied ? 0.0 : shadow;
}

void renderBuilding(inout vec3 col, vec2 p, LotInfo L, float tile, float aa){
    if(lotFilled(L.id) < 0.5) return;

    float h = lotHeight(L.id, tile);
    float layoutF = float(L.layoutId);

    for(int y = 0; y < BOARD_H; y++){
        float fy = float(y);
        for(int x = 0; x < BOARD_W; x++){
            if(!occAt(L.occMask, x, y, L.flipX, L.flipY)) continue;

            bool topOpen   = !occAt(L.occMask, x, y + 1, L.flipX, L.flipY);
            bool rightOpen = !occAt(L.occMask, x + 1, y, L.flipX, L.flipY);

            vec2 corner = L.origin + vec2(float(x), fy) * tile;

            int piece = pieceAtBoardCell(L.layoutId, x, y, L.flipX, L.flipY);
            float pieceF = float(piece);

            vec3 base = pieceColor(L.id, piece);
            base *= mix(0.92, 1.04, hash21(L.id + vec2(pieceF * 2.13, layoutF * 0.97)));

            if(topOpen){
                float edge;
                float m = leftMask(p, corner, h, tile, aa, edge);
                float rim = m * (1.0 - smoothstep(aa * 1.1, aa * 2.8, edge));
                col = mix(col, base * vec3(1.00, 0.82, 0.74) * 0.72, m);
                col = mix(col, vec3(0.06, 0.07, 0.10), rim * 0.9);
            }

            if(rightOpen){
                float edge;
                float m = rightMask(p, corner, h, tile, aa, edge);
                float rim = m * (1.0 - smoothstep(aa * 1.1, aa * 2.8, edge));
                col = mix(col, base * vec3(0.72, 0.90, 1.15) * 0.80, m);
                col = mix(col, vec3(0.06, 0.07, 0.10), rim * 0.9);
            }

            float edge;
            float roof = topMask(p, corner, h, tile, aa, edge);

            float roofFillEdge;
            float roofFill = topMask(
                p,
                corner - vec2(aa * 0.35),
                h,
                tile + aa * 0.70,
                aa,
                roofFillEdge
            );

            vec2 rq = isoInv(p - iso(corner, h));
            float g = clamp((rq.x + rq.y) / (tile * 2.0), 0.0, 1.0);
            float roofShade = mix(0.40, 1.45, pow(g, 0.7));

            vec3 roofCol = min(base * 1.06, vec3(1.0)) * roofShade;

            float roofRim = roof * (1.0 - smoothstep(aa * 1.1, aa * 2.8, edge));
            col = mix(col, roofCol, roofFill);
            col = mix(col, vec3(0.06, 0.07, 0.10), roofRim * 0.9);
        }
    }
}

void main(){
    vec2 fragCoord = v_TexCoord;
    vec2 uv = gl_FragCoord.xy / u_Size.xy;

    uv.x *= u_Size.x / u_Size.y;
    uv.y = 1.0 - uv.y;

    float zoom = 10.0;
    vec2 cam = vec2(u_Time * 1.25, u_Time * 0.85);

    vec2 p = uv * zoom + cam;
    vec2 w = isoInv(p);

    const int R = 3;
    float pitch = 4.15;
    float plotSize = 3.45;
    float tileSize = plotSize / 5.0;
    float aa = zoom / u_Size.y * 1.35;
    float lotReachX = plotSize + tileSize * 1.35;
    float lotReachY = plotSize * 0.5 + tileSize * 2.2;

    vec2 baseId = floor(w / pitch + 0.5);

    vec3 col = renderGround(w, pitch, plotSize, aa);

    float shadow = groundShadowAt(w, baseId, pitch, tileSize, aa);
    col *= 1.0 - shadow * 0.32;

    col *= 1.0 - 0.06 * length(uv);

    for(int band = -2; band <= 2 * R; band++){
        for(int j = -R; j <= R; j++){
            int i = band - j;
            if(i < -R || i > R) continue;

            vec2 id = baseId + vec2(float(i), float(j));
            vec2 lotCenter = id * pitch;
            vec2 lotPos = iso(lotCenter, 0.0);

            if(abs(lotPos.x - p.x) > lotReachX || abs(lotPos.y - p.y) > lotReachY) continue;
            if(lotFilled(id) < 0.5) continue;

            LotInfo L = makeLotInfo(id, pitch, tileSize);
            renderBuilding(col, p, L, tileSize, aa);
        }
    }

    float vig = pow(16.0 * v_TexCoord.x * v_TexCoord.y *
                    (1.0 - v_TexCoord.x) * (1.0 - v_TexCoord.y), 0.08);

    col *= 0.84 + 0.16 * vig;

    FragColor = vec4(sqrt(max(col, 0.0)), 1.0);
}
)";


static const std::string GHOST_FRAG = R"(#version 330 core
#define NUM_OCTAVES 16

in vec2 v_TexCoord;
in vec2 v_OneTexel;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform vec2 u_Size;
uniform float u_Time;

mat3 rotX(float a) {
    float c = cos(a);
    float s = sin(a);
    return mat3(
        0.5, 0, 0,
        0, c, -s,
        0, s, c
    );
}
mat3 rotY(float a) {
    float c = cos(a);
    float s = sin(a);
    return mat3(
        c, 0, -s,
        0, 1, 0,
        s, 0, c
    );
}

float random(vec2 pos) {
    return fract(sin(dot(pos.xy, vec2(1399.9898, 78.233))) * 43758.5453123);
}

float noise(vec2 pos) {
    vec2 i = floor(pos);
    vec2 f = fract(pos);
    float a = random(i + vec2(0.0, 0.0));
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

float fbm(vec2 pos) {
    float v = 0.0;
    float a = 0.5;
    vec2 shift = vec2(100.0);
    mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.5));
    for (int i=0; i<NUM_OCTAVES; i++) {
        v += a * noise(pos);
        pos = rot * pos * 2.0 + shift;
        a *= 0.5;
    }
    return v;
}

void main(void) {
    vec2 p = (gl_FragCoord.xy - u_Size.xy) / min(u_Size.x, u_Size.y);

    float time2 = 0.6 * u_Time / 2.0;

    vec2 q = vec2(0.0);
    q.x = fbm(p + 0.30 * time2);
    q.y = fbm(p + vec2(1.0));
    vec2 r = vec2(0.0);
    r.x = fbm(p + 1.0 * q + vec2(1.2, 3.2) + 0.135 * time2);
    r.y = fbm(p + 1.0 * q + vec2(8.8, 2.8) + 0.126 * time2);
    float f = fbm(p + r);
    vec3 color = mix(
        vec3(0.0, 0.0, 0),
        vec3(1, 0, 0.7),
        clamp((f * f) * 8.0, 0.0, 5.0)
    );

    color = mix(
        color,
        vec3(0, 0, 1),
        clamp(length(q), 0.0, 1.0)
    );


    color = mix(
        color,
        vec3(0.3, 1, 1),
        clamp(length(r.x), 0.0, 1.0)
    );

    color = (f * f * f + 0.6 * f * f + 0.9 * f) * color;

    FragColor = vec4(color, 1.0);
})";

static const std::string FRACTAL_FRAG = R"(#version 330 core

in vec2 v_TexCoord;
in vec2 v_OneTexel;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform vec2 u_Size;
uniform float u_Time;

vec3 palette(float d){
	return mix(vec3(0.2,0.7,0.9),vec3(1.,0.,1.),d);
}

vec2 rotate(vec2 p,float a){
	float c = cos(a);
    float s = sin(a);
    return p*mat2(c,s,-s,c);
}

float map(vec3 p){
    for( int i = 0; i<8; ++i){
        float t = u_Time*0.2;
        p.xz =rotate(p.xz,t);
        p.xy =rotate(p.xy,t*1.89);
        p.xz = abs(p.xz);
        p.xz-=.5;
	}
	return dot(sign(p),p)/5.;
}

vec4 rm (vec3 ro, vec3 rd){
    float t = 0.;
    vec3 col = vec3(0.);
    float d;
    for(float i =0.; i<64.; i++){
		vec3 p = ro + rd*t;
        d = map(p)*.5;
        if(d<0.02){
            break;
        }
        if(d>100.){
        	break;
        }
        //col+=vec3(0.6,0.8,0.8)/(400.*(d));
        col+=palette(length(p)*.1)/(400.*(d));
        t+=d;
    }
    return vec4(col,1./(d*100.));
}
void main()
{
    vec2 uv = (gl_FragCoord.xy - u_Size.xy / 2) / min(u_Size.x, u_Size.y);
	vec3 ro = vec3(0.,0.,-50.);
    ro.xz = rotate(ro.xz,u_Time);
    vec3 cf = normalize(-ro);
    vec3 cs = normalize(cross(cf,vec3(0.,1.,0.)));
    vec3 cu = normalize(cross(cf,cs));
    
    vec3 uuv = ro+cf*3. + uv.x*cs + uv.y*cu;
    
    vec3 rd = normalize(uuv-ro);
    
    vec4 col = rm(ro,rd);
    
    
    FragColor = col;
}
)";

static const std::string HAPPY_FRAG = R"(#version 330 core

//https://www.shadertoy.com/view/XsBXWt

in vec2 v_TexCoord;
in vec2 v_OneTexel;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform vec2 u_Size;
uniform float u_Time;

//#define NYAN 
#define WAVES
//#define BORDER

#define RAY_STEPS 150

#define BRIGHTNESS 1.2
#define GAMMA 1.4
#define SATURATION .65


#define detail .001
#define t u_Time*.5


const vec3 origin=vec3(-1.,.7,0.);
float det=0.0;


// 2D rotation function
mat2 rot(float a) {
	return mat2(cos(a),sin(a),-sin(a),cos(a));	
}

// "Amazing Surface" fractal
vec4 formula(vec4 p) {
		p.xz = abs(p.xz+1.)-abs(p.xz-1.)-p.xz;
		p.y-=.25;
		p.xy*=rot(radians(35.));
		p=p*2./clamp(dot(p.xyz,p.xyz),.2,1.);
	return p;
}

// Distance function
float de(vec3 pos) {
#ifdef WAVES
	pos.y+=sin(pos.z-t*6.)*.15; //waves!
#endif
	float hid=0.;
	vec3 tpos=pos;
	tpos.z=abs(3.-mod(tpos.z,6.));
	vec4 p=vec4(tpos,1.);
	for (int i=0; i<4; i++) {p=formula(p);}
	float fr=(length(max(vec2(0.),p.yz-1.5))-1.)/p.w;
	float ro=max(abs(pos.x+1.)-.3,pos.y-.35);
		  ro=max(ro,-max(abs(pos.x+1.)-.1,pos.y-.5));
	pos.z=abs(.25-mod(pos.z,.5));
		  ro=max(ro,-max(abs(pos.z)-.2,pos.y-.3));
		  ro=max(ro,-max(abs(pos.z)-.01,-pos.y+.32));
	float d=min(fr,ro);
	return d;
}


// Camera path
vec3 path(float ti) {
	ti*=1.5;
	vec3  p=vec3(sin(ti),(1.-sin(ti*2.))*.5,-ti*5.)*.5;
	return p;
}

// Calc normals, and here is edge detection, set to variable "edge"

float edge=0.;
vec3 normal(vec3 p) { 
	vec3 e = vec3(0.0,det*5.,0.0);

	float d1=de(p-e.yxx),d2=de(p+e.yxx);
	float d3=de(p-e.xyx),d4=de(p+e.xyx);
	float d5=de(p-e.xxy),d6=de(p+e.xxy);
	float d=de(p);
	edge=abs(d-0.5*(d2+d1))+abs(d-0.5*(d4+d3))+abs(d-0.5*(d6+d5));//edge finder
	edge=min(1.,pow(edge,.55)*15.);
	return normalize(vec3(d1-d2,d3-d4,d5-d6));
}


// Used Nyan Cat code by mu6k, with some mods

vec4 rainbow(vec2 p)
{
	float q = max(p.x,-0.1);
	float s = sin(p.x*7.0+t*70.0)*0.08;
	p.y+=s;
	p.y*=1.1;
	
	vec4 c;
	if (p.x>0.0) c=vec4(0,0,0,0); else
	if (0.0/6.0<p.y&&p.y<1.0/6.0) c= vec4(255,43,14,255)/255.0; else
	if (1.0/6.0<p.y&&p.y<2.0/6.0) c= vec4(255,168,6,255)/255.0; else
	if (2.0/6.0<p.y&&p.y<3.0/6.0) c= vec4(255,244,0,255)/255.0; else
	if (3.0/6.0<p.y&&p.y<4.0/6.0) c= vec4(51,234,5,255)/255.0; else
	if (4.0/6.0<p.y&&p.y<5.0/6.0) c= vec4(8,163,255,255)/255.0; else
	if (5.0/6.0<p.y&&p.y<6.0/6.0) c= vec4(122,85,255,255)/255.0; else
	if (abs(p.y)-.05<0.0001) c=vec4(0.,0.,0.,1.); else
	if (abs(p.y-1.)-.05<0.0001) c=vec4(0.,0.,0.,1.); else
		c=vec4(0,0,0,0);
	c.a*=.8-min(.8,abs(p.x*.08));
	c.xyz=mix(c.xyz,vec3(length(c.xyz)),.15);
	return c;
}

/*
vec4 nyan(vec2 p)
{
	vec2 uv = p*vec2(0.4,1.0);
	float ns=3.0;
	float nt = u_Time*ns; nt-=mod(nt,240.0/256.0/6.0); nt = mod(nt,240.0/256.0);
	float ny = mod(u_Time*ns,1.0); ny-=mod(ny,0.75); ny*=-0.05;
	vec4 color = texture(iChannel1,vec2(uv.x/3.0+210.0/256.0-nt+0.05,1.-.5+uv.y+ny));
	if (uv.x<-0.3) color.a = 0.0;
	if (uv.x>0.2) color.a=0.0;
	if (uv.y>.3) color.a=0.0;
	if (uv.y<-.3) color.a=0.0;
	return color;
}
*/

// Raymarching and 2D graphics

vec3 raymarch(in vec3 from, in vec3 dir) 

{
	edge=0.;
	vec3 p, norm;
	float d=100.;
	float totdist=0.;
	for (int i=0; i<RAY_STEPS; i++) {
		if (d>det && totdist<25.0) {
			p=from+totdist*dir;
			d=de(p);
			det=detail*exp(.13*totdist);
			totdist+=d; 
		}
	}
	vec3 col=vec3(0.);
	p-=(det-d)*dir;
	norm=normal(p);
#ifdef SHOWONLYEDGES
	col=1.-vec3(edge); // show wireframe version
#else
	col=(1.-abs(norm))*max(0.,1.-edge*.8); // set normal as color with dark edges
#endif		
	totdist=clamp(totdist,0.,26.);
	dir.y-=.02;
	float sunsize = 6.5 + 0.5*sin(u_Time*0.2);
	float an=atan(dir.x,dir.y)+u_Time*1.5; // angle for drawing and rotating sun
	float s=pow(clamp(1.0-length(dir.xy)*sunsize-abs(.2-mod(an,.4)),0.,1.),.1); // sun
	float sb=pow(clamp(1.0-length(dir.xy)*(sunsize-.2)-abs(.2-mod(an,.4)),0.,1.),.1); // sun border
	float sg=pow(clamp(1.0-length(dir.xy)*(sunsize-4.5)-.5*abs(.2-mod(an,.4)),0.,1.),3.); // sun rays
	float y=mix(.45,1.2,pow(smoothstep(0.,1.,.75-dir.y),2.))*(1.-sb*.5); // gradient sky
	
	// set up background with sky and sun
	vec3 backg=vec3(0.5,0.,1.)*((1.-s)*(1.-sg)*y+(1.-sb)*sg*vec3(1.,.8,0.15)*3.);
		 backg+=vec3(1.,.9,.1)*s;
		 backg=max(backg,sg*vec3(1.,.9,.5));
	
	col=mix(vec3(1.,.9,.3),col,exp(-.004*totdist*totdist));// distant fading to sun color
	if (totdist>25.) col=backg; // hit background
	col=pow(col,vec3(GAMMA))*BRIGHTNESS;
	col=mix(vec3(length(col)),col,SATURATION);
#ifdef SHOWONLYEDGES
	col=1.-vec3(length(col));
#else
	col*=vec3(1.,.9,.85);
#ifdef NYAN
	dir.yx*=rot(dir.x);
	vec2 ncatpos=(dir.xy+vec2(-3.+mod(-t,6.),-.27));
	vec4 ncat=nyan(ncatpos*5.);
	vec4 rain=rainbow(ncatpos*10.+vec2(.8,.5));
	if (totdist>8.) col=mix(col,max(vec3(.2),rain.xyz),rain.a*.9);
	if (totdist>8.) col=mix(col,max(vec3(.2),ncat.xyz),ncat.a*.9);
#endif
#endif
	return col;
}

// get camera position
vec3 move(inout vec3 dir) {
	vec3 go=path(t);
	vec3 adv=path(t+.7);
	float hd=de(adv);
	vec3 advec=normalize(adv-go);
	float an=adv.x-go.x; an*=min(1.,abs(adv.z-go.z))*sign(adv.z-go.z)*.7;
	dir.xy*=mat2(cos(an),sin(an),-sin(an),cos(an));
    an=advec.y*1.7;
	dir.yz*=mat2(cos(an),sin(an),-sin(an),cos(an));
	an=atan(advec.x,advec.z);
	dir.xz*=mat2(cos(an),sin(an),-sin(an),cos(an));
	return go;
}

void main()
{
	vec2 uv = gl_FragCoord.xy / u_Size.xy*2.-1.;
	vec2 oriuv=uv;
	uv.y*=u_Size.y/u_Size.x;
	vec2 mouse = vec2(0.0, 0.0);
	float fov=.9-max(0.,.7-u_Time*.3);
	vec3 dir=normalize(vec3(uv*fov,1.));
	dir.yz*=rot(mouse.y);
	dir.xz*=rot(mouse.x);
	vec3 from=origin+move(dir);
	vec3 color=raymarch(from,dir); 
	#ifdef BORDER
	color=mix(vec3(0.),color,pow(max(0.,.95-length(oriuv*oriuv*oriuv*vec2(1.05,1.1))),.3));
	#endif
	FragColor = vec4(color,1.);
})";

static const std::string MOBIUS_SPHERE_FRAG = R"(#version 330 core

//https://www.shadertoy.com/view/7fl3DX

in vec2 v_TexCoord;
in vec2 v_OneTexel;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform vec2 u_Size;
uniform float u_Time;
uniform int u_FrameCount;

// Cotterzz's raytraced sphere fix: The standard function most 
// of us use doesn't really cater for miniscule spheres. If speed
// was a concern and the spheres were larger (most of the time,
// they are), you could use the regular one.
float traceSphere(in vec3 ro, in vec3 rd, in vec4 sph){

    vec3 oc = ro - sph.xyz;
	float b = dot(oc, rd);
    if(b>0.) return 1e8;
    // OLD: catastrophic cancellation near silhouette edges
    // float c = dot(oc, oc) - sph.w*sph.w;
    // float h = b*b - c;

    // NEW: h = r² - |oc × rd|²  (stable, no large-minus-large)
    vec3 cx = cross(oc, rd);
    float h = sph.w*sph.w - dot(cx, cx);

    if(h < 0.) return 1e8;
    return -b - sqrt(h);    
}
 
// Plane intersection: Old formula, and could do with some tidying up.
// The tiny "9e-7" figure is something I hacked in to stop near plane 
// artifacts from appearing. I don't like it at all, but not a single 
// formula I found deals with the problem. There definitely has to be
// a better way, so if someone knows of a more robust formula, I'd 
// love to use it.
float tracePlane(vec3 ro, vec3 rd, vec3 n, vec3 o){


    float t = 1e8;
 
	float ndotdir = dot(rd, n);
     
	if (ndotdir<0.){
	
		float dist = -(dot(ro - o, n) + 9e-7*0.)/ndotdir;	// + 9e-7
   		
		if (dist>0.){ 
            t = dist; 
  		}
	}
    
    return t;

}

// This example only works with a flat top arrangement, but
// I'll arrange for it to work with both later.
#define FLAT_TOP_HEXAGON

// Helper vector. If you're doing anything that involves regular triangles or hexagons, the
// 30-60-90 triangle will be involved in some way, which has sides of 1, sqrt(3) and 2.
#ifdef FLAT_TOP_HEXAGON
const vec2 s = vec2(1.7320508, 1)/2.;
#else
const vec2 s = vec2(1, 1.7320508)/2.;
#endif


// The 2D hexagonal isosuface function: If you were to render a horizontal line and one that
// slopes at 60 degrees, mirror, then combine them, you'd arrive at the following. As an 
// aside, the function is a bound -- as opposed to a Euclidean distance representation, but 
// either way, the result is hexagonal boundary lines.
float hex(in vec2 p){
    
    p = abs(p);
    
    #ifdef FLAT_TOP_HEXAGON
    // Below is equivalent to:
    //return max(p.x*.866025 + p.y*.5, p.y); 

    return max(dot(p, vec2(1.7320508, 1)/2.), p.y); // Hexagon.
    #else
    // Below is equivalent to:
    //return max(p.x*.5 + p.y*.866025, p.x); 

    return max(dot(p, vec2(1, 1.7320508)/2.), p.x); // Hexagon.
    #endif
    
}

// The hexagon grid.
//
// This function returns the hexagonal grid coordinate for the grid cell, and the 
// corresponding hexagon cell ID -- in the form of the central hexagonal point. That's 
// basically all you need to produce a hexagonal grid.
//
// When working with 2D, I guess it's not that important to streamline this particular 
// function. However, if you need to raymarch a hexagonal grid, the number of operations 
// tend to matter. This one has minimal setup, one "floor" call, a couple of "dot" calls, 
// a ternary operator, etc. To use it to raymarch, you'd have to double up on everything -- 
// in order to deal with overlapping fields from neighboring cells, so the fewer operations 
// the better.
vec4 getHex(vec2 p){
    
    // The two mutually offset coordinate systems. One for each hexagon.
    //
    // Two sets of repeat hexagons are required to fill in the space, and the two 
    // sets are stored in a "vec4" in order to group some calculations together. 
    // The hexagon center we'll eventually use will depend upon which is closest to the 
    // current point. Since the central hexagon point is unique, it doubles as the unique
    // hexagon ID.
    vec4 h = vec4(p, p - s/2.);
    // Their respective IDs. iC*s.xyxy represent the cell centers.
    vec4 iC = floor(h/s.xyxy) + .5;
     
    // Centering the coordinates with hexagon centers above to
    // produce respective local coordinates.
    h -= iC*s.xyxy; 
 
    // Determine the nearest hexagon cell, then return the local coordinates
    // and the integer IDs. Multiplying the ID by "s" will give you the
    // position based hexagon center.
    return dot(h.xy, h.xy)<dot(h.zw, h.zw)? vec4(h.xy, iC.xy) : vec4(h.zw, iC.zw + .5);     

}


///// Variable defines.

// The grid element. The circles almost look like a mapped
// spherical Doyle spiral, but not quite... I have one of 
// those that I'll post later.
//
// Grid shape: Hexagons: 0, Circles: 1.
#define SHAPE 0
   
// Display the sphere.
#define SHOW_SPHERE
 
////////////////////


#define PI 3.14159265358979323846
#define TAU 6.28318530717958647693

// Max ray distance.
#define FAR 20.


// Scene object ID.
int objID;


// Standard 2D rotation formula.
mat2 rot2(in float a){ float c = cos(a), s = sin(a); return mat2(c, s, -s, c); }

// Hash without Sine -- Dave Hoskins
// https://www.shadertoy.com/view/4djSRW
// 1 out, 2 in...
float hash21(vec2 p) {
 
    p = fract(p*vec2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return fract(p.x*p.y);
}

// IQ's box function.
float sBoxS(in vec2 p, in vec2 b, in float rf){
  
  vec2 d = abs(p) - b + rf;
  return min(max(d.x, d.y), 0.) + length(max(d, 0.)) - rf;
    
}

 
// Object distance holder. We're only using two, so this
// is a bit of overkill.
vec4 vObj;

// The extruded image.
float map(vec3 p){
    
    // Floor.
    float fl = p.y + .5;
     
    // Sphere.
    #ifdef SHOW_SPHERE
    float sph = length(p) - .5;
    #else
    float sph = 1e5;
    #endif
      
       
    // Object IDs.
    vObj = vec4(fl, sph, 1e5, 1e5);
    
    // Minimum scene distance.
    return min(fl, sph); 
 
}

 
// Basic raymarcher.
float trace(in vec3 ro, in vec3 rd){

    // Overall ray distance and scene distance.
    float t = 0., d;
    
    for(int i = min(0, u_FrameCount); i<128; i++){
    
        d = map(ro + rd*t);
        // For stereographic projection, accuracy at the poles matter, so there
        // is the extra condition that the distance be negative. Without it, 
        // a kind of singularity artifact will appear at the pole. There'd be a 
        // reason for it, but I'm not sure what it would be... Something to do 
        // with sciece would be ny guess. :D 
        if((abs(d)<.001 && d<0.) || t>FAR) break;
        
        t += d*.9; 
    }

    return min(t, FAR);
}


// Standard normal function. It's not as fast as the tetrahedral calculation, 
// but more symmetrical.
vec3 getNormal(in vec3 p, float t) {
	const vec2 e = vec2(.001, 0);
	return normalize(vec3(map(p + e.xyy) - map(p - e.xyy), map(p + e.yxy) - map(p - e.yxy),	
                          map(p + e.yyx) - map(p - e.yyx)));
}


// Cheap shadows are hard. In fact, I'd almost say, shadowing particular scenes with
// limited iterations is impossible... However, I'd be very grateful if someone could 
// prove me wrong. :)
float softShadow(vec3 ro, vec3 lp, vec3 n, float k){

    // More would be nicer. More is always nicer, but not always affordable. :)
    const int maxIterationsShad = 32; 
    
    ro += n*.0015; // Coincides with the hit condition in the "trace" function.  
    vec3 rd = lp - ro; // Unnormalized direction ray.

    float shade = 1.;
    float t = 0.; 
    float end = max(length(rd), .0001);
    //float stepDist = end/float(maxIterationsShad);
    rd /= end;

    // Max shadow iterations - More iterations make nicer shadows, but slow things down. 
    // Obviously, the lowest number to give a decent shadow is the best one to choose. 
    for (int i = min(u_FrameCount, 0); i<maxIterationsShad; i++){

        float d = map(ro + rd*t);
        shade = min(shade, k*d/t);
        //shade = min(shade, smoothstep(0., 1., k*d/t)); // Thanks to IQ for this tidbit.
        // So many options here, and none are perfect: dist += clamp(d, .01, stepDist), etc.
        t += clamp(d, .005, .15); 
        
        
        // Early exits from accumulative distance function calls tend to be a good thing.
        if (d<0. || t>end) break; 
    }

    // Cap above zero.
    return max(shade, 0.); 
}


// I keep a collection of occlusion routines... OK, that sounded really nerdy. :)
// Anyway, I like this one. I'm assuming it's based on IQ's original.
float calcAO(in vec3 p, in vec3 n){

	float sca = 2., occ = 0.;
    for( int i = 0; i<5; i++ ){
    
        float hr = float(i + 1)*.15/5.;        
        float d = map(p + n*hr);
        occ += (hr - d)*sca;
        sca *= .7;
    }
    
    return clamp(1. - occ, 0., 1.);  
}

// Standard complex transform functions. 
vec2 cmul(vec2 a, vec2 b){ return mat2(a, -a.y, a.x)*b; }
vec2 cinv(vec2 a){ return vec2(a.x, -a.y)/dot(a, a); }
vec2 cdiv(vec2 a, vec2 b){ return cmul(a, cinv(b)); }
vec2 clog(in vec2 z){ return vec2(log(length(z)), atan(z.y, z.x)); }
vec2 cexp(vec2 z){ return exp(z.x)*vec2(cos(z.y), sin(z.y)); }
vec2 cpow(vec2 a, vec2 b){ return cexp(cmul(b, clog(a))); }


// Rolling the ball on the plane.
vec3 rollObj(vec3 p){

    //p.xy *= rot2(u_Time/4.);
    p.xz *= rot2(u_Time/2.);
    p.yz *= rot2(u_Time/1.);
    return p;
}

 

// Stereographically maps the unit sphere sittin on a plane
// to the plane itself.
vec2 stereographic(vec3 p){
 
    return p.xz / (1. - p.y);
}

// Reverse stereographic mapping of the plane to a unit sphere
// sitting on it.
vec3 stereographicInverse(vec2 p) {

    float r2 = dot(p, p);
    return vec3(2. * p.x, r2 - 1., 2. * p.y) / (1. + r2);
}



// Floor. 
vec2 fFloor(vec3 p) {

    // Floor.    

    // Move the pattern to the ball, rotate it, then 
    // project it back down to the floor.    
    p = stereographicInverse(p.xz);
    p = rollObj(p);
    return stereographic(p);

}

// Spiral repetition factor. Positive integers will work.
vec2 rep = vec2(4, 8);

vec3 transform(vec3 p) {

    // If there were no rolling movement, you wouldn't need this.
    p.xz = fFloor(p);


    // 2D plane transformations.

    // The three lines below will produce a standard Mobius spiral
    // pattern. Commenting in the other two lines will produce something
    // more interesting, but I figured it was a bit much for this 
    // example.
    //p.xz = cpow(p.xz - vec2(1, .5), vec2(3., 0));   

#if 1
// Mobius transformation -- Probably the simplest one.
    p.xz = cdiv(p.xz - vec2(1, 0), p.xz + vec2(1, 0));
    // p.xz += cdiv(p.xz - vec2(.5, 0).yx, p.xz + vec2(.5, 0).yx);
    // This turns the Mobius transform (above) into a double spiral.
    p.xz = clog(p.xz);
#else
// Something more interesting -- Cool looking, but a bit much for this example.
    float N = 2.; // N-tuples of spirals... 3, 4, etc.
    p.xz = clog(cdiv(vec2(2, 0), cpow(p.xz, vec2(N, 0)) - vec2(1, 0)) + vec2(1, 0));
#endif

    // The "rep" factor controls how the spiral looks.
    p.xz = cmul(p.xz, rep * vec2(1, sqrt(3.) / 2.) / TAU);


    return p;

}


// Numeric transform function derivative. You could determine this analytically.
// In fact, I usually do, but this allows you try out more interesting complex
// transform combinations.
vec3 funcD(vec3 p) {

    // Numeric derivative.
    float px = 1e-4;
    vec3 f = transform(p);
    vec3 dtX = (transform(p + vec3(px, 0, 0)) - f) / px;
    vec3 dtY = (transform(p + vec3(0, px, 0)) - f) / px;
    vec3 dtZ = (transform(p + vec3(0, 0, px)) - f) / px;
    //return vec3(length(dtX), length(dtY), length(dtZ))/sqrt(3.);
    return (mat3(dtX, dtY, dtZ) * vec3(1)) / sqrt(3.);

}

// Decorative circle distances... Added in at the last minutes.
float gCir;
float gCir2;

vec3 getPattern(vec3 p3) {

    // Pattern scale.
    vec2 sc = vec2(1) / 2.;

    // Transform derivative. Used to produce border lines of equal
    // width... Not technically correct, but close enough for this example.
    float tdF = length(funcD(p3));

    // Perform the complex transform. In this case, a
    // Mobius spiral.
    p3 = transform(p3);

    // Floor (2D grid) coordinates.
    vec2 p = p3.xz;

    // Produce the hexagon grid.
    vec4 p4 = getHex(p);
    // Local coordiantes and ID.
    p = p4.xy;
    vec2 ip = p4.zw;


    // Grid shape: Circles or hexagons.
#if SHAPE == 1
// Circles.
    float poly = length(p) - s.y / 2.;
#else 
    float poly = hex(p) - s.y / 2.;
#endif


    // Cut out a dot in the center.
    poly = max(poly, -(length(p) - sc.x / 32.));

    // Divide by the transform derivative. This will give you equal
    // cell border widths, in a lot of cases.
    poly /= tdF;


    // Last minute circle functions, to use for decorative
    // purposes. Not really necessary.
    vec2 offs = s * vec2(1, -1); // Offsets.
    vec2 offs2 = s * vec2(1, -1) / 8.;

    gCir = -(length(p - offs2) - s.y / 2. / .866);
    gCir /= tdF;

    gCir2 = length(p - offs2.yx / 2.) - sc.x / 5.;
    gCir2 /= tdF;


    // Cell distance and cell ID.
    return vec3(poly, ip);
}

/*
// Planar to spherical camera. Not quite, but close enough.
vec3 sphereCam(in vec2 p){

    //return normalize(vec3(p, 1)); // Debug.

    float t = 1./(1. + dot(p,p)/3.);
    return vec3(p*t, 2.*t - 1.);
}
*/

void main() {


    // Screen coordinates.
    vec2 uv = (gl_FragCoord.xy - u_Size * .5) / u_Size.y;


    // Camera Setup.
    vec3 ro = vec3(0, 1.5, -2); // Camera position, doubling as the ray origin.
    vec3 lk = vec3(0, -.25, 0);//vec3(0, -.25, u_Time);  // "Look At" position.

    // Light position. 
    vec3 lp = ro + vec3(.75, 0, 1.5);// Put it a bit in front of the camera.


    // Using the above to produce the unit ray-direction vector.
    float FOV = .85; // FOV - Field of view.
    vec3 fwd = normalize(lk - ro); // Forward.
    vec3 rgt = normalize(cross(vec3(0, 1, 0), fwd));// Right. 
    // "right" and "forward" are perpendicular normals, so the result is normalized.
    vec3 up = cross(fwd, rgt); // Up.


    // rd - Ray direction.  
    vec3 rd = normalize(uv.x * rgt + uv.y * up + fwd / FOV);
    // Camera.
    //mat3 cam = mat3(rgt, up, fwd);
    // rd - Ray direction.
    //vec3 rd = cam*normalize(vec3(uv, 1./FOV));
    // A bit of lens mutation to increase the scene peripheral, if that's your thing.
    //vec3 rd = cam*sphereCam(uv*PI*.2/FOV);




    // Raymarch to the scene.
    float t = trace(ro, rd);

    // Save the object ID.
    float minDist = 1e5;

    // Only two objects. Identify the closest one.
    objID = vObj.x < vObj.y ? 0 : 1;

    /*
    // Sorting four objects. Not needed here.
    objID = 0;
    for(int i = 0; i<4; i++){
       if(vObj[i]<minDist){
           minDist = vObj[i];
           objID = i;
       }
    }
    */


    // Initiate the scene color to black.
    vec3 col = vec3(0);

    // The ray has effectively hit the surface, so light it up.
    if (t < FAR) {


        // Surface position and surface normal.
        vec3 sp = ro + rd * t;
        vec3 sn = getNormal(sp, t);

        // Light direction vector.
        vec3 ld = lp - sp;

        // Distance from respective light to the surface point.
        float lDist = max(length(ld), .001);

        // Normalize the light direction vector.
        ld /= lDist;



        // Shadows and ambient self shadowing.
        float sh = softShadow(sp, lp, sn, 16.);
        float ao = calcAO(sp, sn); // Ambient occlusion.
        //sh = min(sh + ao*.25, 1.);

        // Light attenuation, based on the distances above.
        float atten = 1. / (1. + lDist * .05);


        // Diffuse lighting.
        float diff = max(dot(sn, ld), 0.);
        //diff = pow(diff, 4.)*2.; // Ramping up the diffuse.

        // Specular lighting.
        float spec = pow(max(dot(reflect(ld, sn), rd), 0.), 32.);

        // Fresnel term. Good for giving a surface a bit of a reflective glow.
        //float fre = pow(clamp(1. - abs(dot(sn, rd))*.5, 0., 1.), 2.);

        // Schlick approximation. I use it to tone down the specular term. It's pretty subtle,
        // so could almost be aproximated by a constant, but I prefer it. Here, it's being
        // used to give a hard clay consistency... It "kind of" works.
        float Schlick = pow(1. - max(dot(rd, normalize(rd + ld)), 0.), 5.);
        float freS = mix(.15, 1., Schlick);  //F0 = .2 - Glass... or close enough. 


        // Obtaining the texel color. 
        vec3 texCol = vec3(.6);

        // Texture coordinates.
        vec3 txP = sp;

        // Smooth factor and edge width.
        float sf = .005;
        float ew = .018;

        // Pattern variable: Distance and ID.
        vec3 d3;

        // Object coloring.
        if (objID > 0) {

            // Ball. 

            ////
            // Raytrace from the ball to the floor, then obtain the 
            // Mobius spiral pattern value.

            // North pole to hit point unit direction vector.
            vec3 rd2 = normalize(txP - vec3(0, .5, 0));
            // Use it to trace from the hit point to the plane.
            float t2 = tracePlane(txP, rd2, vec3(0, 1, 0), vec3(0, -.5, 0));
            // Intersection point on the plane.
            vec3 txP2 = txP + rd2 * t2;
            // Obtain the pattern values for coloring: Distance and ID.
            d3 = getPattern(txP2);
            ////

            // Applying a hack to approximate equi-spaced lines... I was in a 
            // hurry... I haven't decided whether a precise calculation is possible.
            // Either way, this looks OK for the purpose of this demonstration.
            ew *= 2. / length(txP - vec3(0, 1, 0));
            //ew *= 2.*pow(1. + txP.y, 2.);

        }
        else {

            // Floor.
            d3 = getPattern(txP);

        }

        // Wrapping the cell ID with the Mobius spiral repetition factor.
        vec2 id = mod(d3.yz, rep.yx) / (rep.x * rep.y);

        // Rand, or non-random ID value.
        float rnd = hash21(id + .1);
        //rnd = (d3.y + d3.z*4.)/(4.*4.);
        //rnd = id.y + id.x*rep.y;
        //rnd = id.x*rep.x + id.y*rep.y;
        rnd = dot(id, rep);

        // Use the ID to color the cell in some way.
        // Two slightly different shades.
        vec3 cCol = .5 + .45 * cos(TAU * rnd + vec3(0, PI / 2., PI));
        vec3 cCol2 = .5 + .45 * cos(TAU * rnd + .25 + vec3(0, PI / 2., PI));
        // Order the two shades according to brightness.
        if (dot(cCol2 - cCol, vec3(299, .587, .114)) < 0.) {
            vec3 tmp = cCol; cCol = cCol2; cCol2 = tmp;
        }
        // Render.
        cCol = mix(cCol, cCol2 * 1.2 + .05, 1. - smoothstep(0., sf * 2., gCir));
        cCol = mix(cCol, cCol * 1.5 + .1,
            1. - smoothstep(0., sf * 2., abs(gCir + ew / 4.) - ew / 4.));


        // Background.
        texCol = vec3(.0);
        // Cell color.
        texCol = mix(texCol, cCol, 1. - smoothstep(0., sf, d3.x + ew));


        // Rendering an edge ring around the sphere.
        //
        // Adapted from one of IQ's world to screen space examples. 
        // I'll track down the particular one later.
#ifdef SHOW_SPHERE
        mat3 cam = mat3(rgt, up, fwd);
        mat4 cam4 = mat4(rgt, 0, up, 0, fwd, 0, ro, 1.);
        mat4 invCam = inverse(cam4);
        vec3 qq = (invCam * vec4(vec3(0), 1.)).xyz;
        // 2D Screen space.
        vec2 s = (uv - qq.xy / qq.z / FOV) * qq.z;
        float r = .5 / FOV;
        texCol = mix(texCol, vec3(0),
            1. - smoothstep(0., .003, abs(length(s) - r - .015) - .01));
#endif



        // Combining the above terms to produce the final color.
        col = texCol * (diff * sh + .3 + vec3(1, .97, .92) * spec * freS * 2. * sh);

        // Extra light.
        //col += col/(1. + lDist*lDist);

        // Shading.
        col *= ao * atten;


    }


    // Horizon fog. Not visible here, but provided for completeness.
    col = mix(col, vec3(0), smoothstep(0., .99, t / FAR));


    // Subtle vignette. Designers use them to frame things and guide
    // the viewer's eyes toward the center... or something like that.
    vec2 w = vec2(u_Size.x / u_Size.y, 1);
    col *= 1.05 - smoothstep(0., .1, sBoxS(uv, w / 2., .15) + .1) * .15;


    // Rought gamma correction.
    FragColor = vec4(sqrt(max(col, 0.)), 1);

}
)";

static const std::string HEXAGON_FRAG = R"(#version 330 core

//https://www.shadertoy.com/view/fsdcz7

in vec2 v_TexCoord;
in vec2 v_OneTexel;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform vec2 u_Size;
uniform float u_Time;
uniform int u_FrameCount;

// Using a hexagon grid based pattern, instead of a square one.
#define HEXAGON

// Random cell subdivsion: Commenting this out would mean compulsory cell 
// subdivision. Whether you leave this in or not depends on the look you're after.
#define RANDOM_SUBDIVISION

// Thick outer cell borders for a more cartoonish look.
//#define THICK_BORDER
        

// Standard 2D rotation formula.
mat2 rot2(in float a){ float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }


// IQ's vec2 to float hash.
float hash21(vec2 p){  return fract(sin(dot(p, vec2(27.619, 57.583)))*43758.5453); }


float sBoxS(in vec2 p, in vec2 b, in float rf){
  
  vec2 d = abs(p) - b + rf;
  return min(max(d.x, d.y), 0.) + length(max(d, 0.)) - rf;
    
}

// Flat top or pointed top hexagon.
#define FLAT_TOP_HEXAGON
//
// Helper vector. If you're doing anything that involves regular triangles or hexagons, the
// 30-60-90 triangle will be involved in some way, which has sides of 1, sqrt(3) and 2.
#ifdef FLAT_TOP_HEXAGON
vec2 s = vec2(1.7320508, 1);
#else
vec2 s = vec2(1, 1.7320508);
#endif


// The 2D hexagonal isosuface function: If you were to render a horizontal line and one that
// slopes at 60 degrees, mirror, then combine them, you'd arrive at the following. As an aside,
// the function is a bound -- as opposed to a Euclidean distance representation, but either
// way, the result is hexagonal boundary lines.
float hex(in vec2 p){
    
    p = abs(p);
    
    #ifdef FLAT_TOP_HEXAGON
    // Below is equivalent to:
    //return max(p.x*.866025 + p.y*.5, p.y); 

    return max(dot(p, vec2(1.7320508, 1)*.5), p.y); // Hexagon.
    #else
    // Below is equivalent to:
    //return max(p.x*.5 + p.y*.866025, p.x); 

    return max(dot(p, vec2(1, 1.7320508)*.5), p.x); // Hexagon.
    #endif
    
}

// This function returns the hexagonal grid coordinate for the grid cell, and the corresponding 
// hexagon cell ID -- in the form of the central hexagonal point. That's basically all you need to 
// produce a hexagonal grid.
//
// When working with 2D, I guess it's not that important to streamline this particular function.
// However, if you need to raymarch a hexagonal grid, the number of operations tend to matter.
// This one has minimal setup, one "floor" call, a couple of "dot" calls, a ternary operator, etc.
// To use it to raymarch, you'd have to double up on everything -- in order to deal with 
// overlapping fields from neighboring cells, so the fewer operations the better.
vec4 getHex(vec2 p){
    
    // The hexagon centers: Two sets of repeat hexagons are required to fill in the space, and
    // the two sets are stored in a "vec4" in order to group some calculations together. The hexagon
    // center we'll eventually use will depend upon which is closest to the current point. Since 
    // the central hexagon point is unique, it doubles as the unique hexagon ID.
    #ifdef FLAT_TOP_HEXAGON
    vec4 hC = floor(vec4(p/s, p/s - vec2(1.7320508/3., .5))) + .5;
    #else
    vec4 hC = floor(vec4(p/s, p/s - vec2(.5, 1.7320508/3.))) + .5;
    #endif    
   
    // Centering the coordinates with the hexagon centers above.
    vec4 h = vec4(p - hC.xy*s, p - (hC.zw + .5)*s);
    //vec4 h = p.xyxy - vec4(hC.xy + .5, hC.zw)*s.xyxy;
    
    
    // Nearest hexagon center (with respect to p) to the current point. In other words, when
    // "h.xy" is zero, we're at the center. We're also returning the corresponding hexagon ID --
    // in the form of the hexagonal central point. By the way, the unique ID (the .zw bit), 
    // needs to be multiplied by "s" to give the correct quantized position back. 
    // For example: float ns = noise2D(hID*s);
    //
    // On a side note, I sometimes compare hex distances, but I noticed that Iomateron compared
    // the squared Euclidian version, which seems neater, so I've adopted that. 
    return dot(h.xy, h.xy)<dot(h.zw, h.zw) ? vec4(h.xy, hC.xy) : vec4(h.zw, hC.zw + .5);

}

// A second offset distance field value. Used for hilighting.
float dHi;

vec3 distField(vec2 p){

    // Set the distance (and highlight distance) to a minimum.
    float d = -1e5;
    dHi = -1e5;
    
    // Overal unique cell ID.
    vec2 gIP = vec2(0);
    
    // Inititalize the scale to one.
    float sc = 1.;
    
    // Directional light.
    vec2 ld = normalize(vec2(1, 1.5));
    
    // Rotate and translate the coordinates and light.
    p *= rot2(u_Time/8.);
    p -= u_Time/16.;
    ld *= rot2(u_Time/8.);
     
   
    // Six subdivided grid partitions.
    for(int i = 0; i<6; i++){
    
        
        // Get the hexagon (or square) grid information
        // (local coordinates and cell ID) for this iteration.
        #ifdef HEXAGON
        vec4 p4 = getHex(p);
        p = p4.xy;
        vec2 ip = p4.zw;
        d = max(d, hex(p) - .5/sc); // Hexagon distance.
        dHi = max(dHi, (hex(p + ld*.001) - .5/sc)); // Highlight distance.
        #else
        vec2 ip = floor(p*sc) + .5;
        p -= ip/sc;
        d = max(d, sBoxS(p, vec2(.5/sc), 0.)); // Square distance.
        dHi = max(dHi, sBoxS(p + ld*.001, vec2(.5/sc), 0.)); // Highlight distance.
        #endif
  
        
  
        // Rescale the grid for the next iteration.
        #ifdef HEXAGON
        sc *= 1.4;
        s /= 1.4;
        #else
        sc *= 1.5;
        #endif
        
        // Update the overall cell ID.
        gIP += ip/sc;    
        
        // Optional random rotation -- It looks more interesting, but
        // it's not mandatory.
        p *= rot2((hash21(gIP + .05) - .5)*6.2831);
        ld *= rot2((hash21(gIP + .05) - .5)*6.2831);
       
        // Random translation.
        p -= vec2(hash21(gIP + .13), hash21(gIP + .04))/sc*.75;
        
        #ifdef RANDOM_SUBDIVISION
        // Random cell subdivsion. Commenting this out would mean compulsory
        // cell subdivision. Whether you leave this in or not depends on what
        // look you're after.
        if(i>2 && hash21(gIP + .22)<.2) break;
        #endif
        
        /*
        // Internal moving rotation -- Interesting, but a bit much.
        float dir = hash21(gIP + .07)<.5? -1. : 1.;
        p *= rot2(dir*u_Time/sc/8.);
        ld *= rot2(dir*u_Time/sc/8.);
        */ 
        
        // Extra temporal translation. Also not mandatory.
        p -= u_Time/32.;
         
        
        
    
    }
   
    // Return the cell distance and unique ID.
    return vec3(d, gIP);
}

void main(){

    // Aspect corret coordinates.
    vec2 uv = (gl_FragCoord.xy - u_Size.xy*.5)/u_Size.y;

    // Scale and smoothing factor.
    const float sc = 1.;
    float sf = sc/u_Size.y;
    
    
    // Scaling and translation.
    vec2 p = sc*uv;
    
    // Scene object -- Returns distance and ID.
    vec3 d = distField(p); 
    
    // Rendering onto the canvas.
    
    // ID based cell coloring.
    float rnd = hash21(d.yz + .1);
    float taper = rnd<.35? .7 : 1.;
    vec3 oCol = .5 + .45*cos(6.2831*hash21(d.yz + .2)/5. + vec3(0, 1, 2)/taper);
    if(rnd>=.35) oCol = mix(oCol.zyx, vec3(1)*dot(oCol, vec3(.299, .587, .114)), .75)/4.;
    //else oCol = oCol.zyx;
    
    // Directional derivative bump map calculation for some highlighting.
    float b = max(dHi - d.x, 0.)/.001;
  
    oCol = oCol*(.25 + b*.75)*1.35;
   
     
    // Diagonal line pattern.
    //
    // Resolution independent line number -- Not PPI independent though.
    float lns = 120.*u_Size.y/450.;
    vec2 rp = rot2(-3.14159/3.)*p;
    float pat = abs(fract((rp.x)*lns) - .5)*2. - .05;
    pat = smoothstep(0., sf*lns*2., pat); 
    
    float ew = .005*450./u_Size.y; // Resolution independent edge width.
    
    // Scene color.
    vec3 col = oCol*(pat*1. + .5);
    
    // Cell border.
    #ifdef THICK_BORDER
    const float bw = .0025; // Border width.
    #else
    const float bw = .001; // Border width.
    #endif
    // Border distance.
    float dBord = abs(d.x + bw) - bw; 
    
    // Application... There are definitely better ways, but I was pushed for time.
    col = mix(col, vec3(0), (1. - smoothstep(0., sf*8., dBord - ew))*.25); // Inner gradient.
    col = mix(col, vec3(0), (1. - smoothstep(0., sf, dBord - ew - .005))*.8); // Inner edge.
    col = mix(col, mix(oCol, vec3(1), .125), 1. - smoothstep(0., sf, dBord - ew)); // Colored edge.
    col = mix(col, vec3(0), 1. - smoothstep(0., sf, dBord)); // Outer edge.
    

    // Vertical color gradient of sorts.
    float grad = clamp(uv.y + .65, 0., 1.);
    col = mix(col.yxz, col.zyx, grad*grad);
    
    // Red to blue colors.
    //col = col.yxz;
    
    // Rough gamma correction and screen presentation.
    FragColor = vec4(sqrt(max(col, 0.)), 1);
    
})";

static const std::string BUBBLE_FRAG = R"(#version 330 core

//https://www.shadertoy.com/view/WdB3Dw

in vec2 v_TexCoord;
in vec2 v_OneTexel;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform vec2 u_Size;
uniform float u_Time;
uniform int u_FrameCount;

// --------------------------------------------------------
// HG_SDF
// https://www.shadertoy.com/view/Xs3GRB
// --------------------------------------------------------

#define PI 3.14159265359

void pR(inout vec2 p, float a) {
    p = cos(a)*p + sin(a)*vec2(p.y, -p.x);
}

float smax(float a, float b, float r) {
    vec2 u = max(vec2(r + a,r + b), vec2(0));
    return min(-r, max (a, b)) + length(u);
}


// --------------------------------------------------------
// Spectrum colour palette
// IQ https://www.shadertoy.com/view/ll2GD3
// --------------------------------------------------------

vec3 pal( in float t, in vec3 a, in vec3 b, in vec3 c, in vec3 d ) {
    return a + b*cos( 6.28318*(c*t+d) );
}

vec3 spectrum(float n) {
    return pal( n, vec3(0.5,0.5,0.5),vec3(0.5,0.5,0.5),vec3(1.0,1.0,1.0),vec3(0.0,0.33,0.67) );
}


// --------------------------------------------------------
// Main SDF
// https://www.shadertoy.com/view/wsfGDS
// --------------------------------------------------------

vec4 inverseStereographic(vec3 p, out float k) {
    k = 2.0/(1.0+dot(p,p));
    return vec4(k*p,k-1.0);
}

float fTorus(vec4 p4) {
    float d1 = length(p4.xy) / length(p4.zw) - 1.;
    float d2 = length(p4.zw) / length(p4.xy) - 1.;
    float d = d1 < 0. ? -d1 : d2;
    d /= PI;
    return d;
}

float fixDistance(float d, float k) {
    float sn = sign(d);
    d = abs(d);
    d = d / k * 1.82;
    d += 1.;
    d = pow(d, .5);
    d -= 1.;
    d *= 5./3.;
    d *= sn;
    return d;
}

float time;

float map(vec3 p) {
    float k;
    vec4 p4 = inverseStereographic(p,k);

    pR(p4.zy, time * -PI / 2.);
    pR(p4.xw, time * -PI / 2.);

    // A thick walled clifford torus intersected with a sphere

    float d = fTorus(p4);
    d = abs(d);
    d -= .2;
    d = fixDistance(d, k);
    d = smax(d, length(p) - 1.85, .2);

    return d;
}


// --------------------------------------------------------
// Rendering
// --------------------------------------------------------

mat3 calcLookAtMatrix(vec3 ro, vec3 ta, vec3 up) {
    vec3 ww = normalize(ta - ro);
    vec3 uu = normalize(cross(ww,up));
    vec3 vv = normalize(cross(uu,ww));
    return mat3(uu, vv, ww);
}

void main() {

    time = mod(u_Time / 2., 1.);

    vec3 camPos = vec3(1.8, 5.5, -5.5) * 1.75;
    vec3 camTar = vec3(.0,0,.0);
    vec3 camUp = vec3(-1,0,-1.5);
    mat3 camMat = calcLookAtMatrix(camPos, camTar, camUp);
    float focalLength = 5.;
    vec2 p = (-u_Size.xy + 2. * gl_FragCoord.xy) / u_Size.y;

    vec3 rayDirection = normalize(camMat * vec3(p, focalLength));
    vec3 rayPosition = camPos;
    float rayLength = 0.;

    float distance = 0.;
    vec3 color = vec3(0);

    vec3 c;

    // Keep iteration count too low to pass through entire model,
    // giving the effect of fogged glass
    const float ITER = 82.;
    const float FUDGE_FACTORR = .8;
    const float INTERSECTION_PRECISION = .001;
    const float MAX_DIST = 20.;

    for (float i = 0.; i < ITER; i++) {

        // Step a little slower so we can accumilate glow
        rayLength += max(INTERSECTION_PRECISION, abs(distance) * FUDGE_FACTORR);
        rayPosition = camPos + rayDirection * rayLength;
        distance = map(rayPosition);

        // Add a lot of light when we're really close to the surface
        c = vec3(max(0., .01 - abs(distance)) * .5);
        c *= vec3(1.4,2.1,1.7); // blue green tint

        // Accumilate some purple glow for every step
        c += vec3(.6,.25,.7) * FUDGE_FACTORR / 160.;
        c *= smoothstep(20., 7., length(rayPosition));

        // Fade out further away from the camera
        float rl = smoothstep(MAX_DIST, .1, rayLength);
        c *= rl;

        // Vary colour as we move through space
        c *= spectrum(rl * 6. - .6);

        color += c;

        if (rayLength > MAX_DIST) {
            break;
        }
    }

    // Tonemapping and gamma
    color = pow(color, vec3(1. / 1.8)) * 2.;
    color = pow(color, vec3(2.)) * 3.;
    color = pow(color, vec3(1. / 2.2));

    FragColor = vec4(color, 1);
}

)";
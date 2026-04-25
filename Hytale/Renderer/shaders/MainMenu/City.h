/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

static constexpr const char* CITY_FRAG = R"(#version 330 core

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
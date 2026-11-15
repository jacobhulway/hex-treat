#include <glad/gl.h>
#include<GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include<vector>
#include<algorithm>
// bit bites ig :|

//hi
//testing tools i think
bool drawHitboxes = false;





struct point {
    float x;
    float y;
    point(float x, float y) : x(x), y(y) {}
    point() {}
};

float playerX;
float playerY;

float playerRadius = 8;

float playerHitboxOffsetX = 0;
float playerHitboxOffsetY = 8;


//shh
float secretPlayerX, secretPlayerY;



//texture stuff      V
unsigned int texture;
using namespace std;
//keep everything here :)
vector<vector<int>> textureStorage = {
    {0, 0, 200, 200},
    {200, 0, 40, 40},
    {222, 0, 200, 200},
    {0, 200, 200, 200},
    {200, 40, 16, 32},
    {216, 40, 6, 6}


}; //write these bite manualy

int getTextureWidth(int ID) {
    return textureStorage[ID][2];
}
int getTextureHeight(int ID) {
    return textureStorage[ID][3];
}

vector<vector<float>> calculatedTextureStorage = {}; //leave this one blank so the function can do the work :o

vector<vector<float>> doTheTextureBites(vector<vector<int>> original, int width, int height) {
    vector<vector<float>> output = {};

    float w = width;
    float h = height;
    for (const vector<int> rect : original) {
        output.push_back({ rect[0] / w, rect[1] / h, rect[2] / w, rect[3] / h });//i hope this works :)
    }
    return output;
}



//use all the formating treats for all of them so its clean ig

int __DIRT__ = 0;
int __ROCK1__ = 1;
int __BUILDING__ = 2;
int __MOSS__ = 3;
int __PLAYER__ = 4;
int __STAR__ = 5;


//is it inificient to put a 1000 by 100000 image on gee pee you? :/
double degreesToRadians(double degrees) {
    return degrees * (3.1416 / 180.0);
}

//playewr darwing stuff right here
struct specialPoint {
    float x;
    float y;
    float z;

    float rx;
    float ry;
    float rz;


};

void initPoint(specialPoint& p, float x, float y, float z) {
    p.x = x;
    p.y = y;
    p.z = z;
    p.rx = x;
    p.ry = y;
    p.rz = z;



}

void rotateSpecialPointAlongZAxis(specialPoint& p, float angle) {
    p.rx = p.x * cos(angle) - p.y * sin(angle);
    p.ry = p.x * sin(angle) + p.y * cos(angle);
    p.rz = p.z;
}

struct pkf {
    float x;
    float y;
    float z;
    specialPoint& p;
    pkf(float x, float y, float z, specialPoint& p) : x(x), y(y), z(z), p(p) {}


};

struct animationKeyFrame {
    vector<pkf> akf;
    animationKeyFrame(vector<pkf> animationFrame) : akf(animationFrame) {}


};

void applyPointKeyFrame(pkf& frame, float strength) {
    frame.p.x += frame.x * strength;
    frame.p.y += frame.y * strength;
    frame.p.z += frame.z * strength;
}
void applyEntireKeyFrame(animationKeyFrame& frame, float strength) {
    for (pkf& f : frame.akf) {
        applyPointKeyFrame(f, strength);
    }
}
struct animation {
    vector<animationKeyFrame> frames;
    float speed = 0.05;
    float frameNumber = 0;
    animation(vector<animationKeyFrame> frames) : frames(frames) {}
    animation() {}
    int frameCount = frames.size()-1;
    void step() {
        frameNumber += speed;
        if (frameNumber > frameCount) {
            frameNumber = 0;
            player.reset();
            cout << player.leftFoot.x << "\n";
        }

    }
    void apply() {
        int f = int(frameNumber);

        applyEntireKeyFrame(frames[f], speed);

    }
};
struct playerModel {
    specialPoint head, butt, leftElbow, leftHand, rightElbow, rightHand, leftKnee, leftFoot, rightKnee, rightFoot;
    
    float angle = 0;
    void reset() {
        initPoint(head, 0, 0, 10);
        initPoint(butt, 0, 0, 0);
        initPoint(rightKnee, 2.5, 0, -7);
        initPoint(rightFoot, 3.5, 0, -14);
        initPoint(leftKnee, -2.5, 0, -7);
        initPoint(leftFoot, -3.5, 0, -14);
        initPoint(rightElbow, 4, 0, 7);
        initPoint(rightHand, 8, 0, 2);
        initPoint(leftElbow, -4, 0, 7);
        initPoint(leftHand, -8, 0, 2);
    }
    playerModel() {
        reset();

    }
    
    animation motion = animation(
        {
            animationKeyFrame({pkf(0, -3, -2, leftKnee), pkf(0, -4, -2, leftFoot), pkf(0, 3, -2, rightKnee), pkf(0, 5, -2, rightFoot)}),
            animationKeyFrame({pkf(0, 3, 0, leftKnee), pkf(0, 5, 0, leftFoot), pkf(0, -3, 2, rightKnee), pkf(0, -6, 2, rightFoot)}),
            animationKeyFrame({pkf(0, 3, 0, leftKnee), pkf(0, 5, 0, leftFoot), pkf(0, -3, 0, rightKnee), pkf(0, -4, 0, rightFoot)}),
            animationKeyFrame({pkf(0, -3, 2, leftKnee), pkf(0, -6, 2, leftFoot), pkf(0, 3, 0, rightKnee), pkf(0, 5, 0, rightFoot)})
        }
    );


    
    void setAngle(float a) {
        angle = a;
        rotateSpecialPointAlongZAxis(head, a);
        rotateSpecialPointAlongZAxis(butt, a);
        rotateSpecialPointAlongZAxis(rightKnee, a);
        rotateSpecialPointAlongZAxis(rightFoot, a);
        rotateSpecialPointAlongZAxis(leftKnee, a);
        rotateSpecialPointAlongZAxis(leftFoot, a);
        rotateSpecialPointAlongZAxis(rightElbow, a);
        rotateSpecialPointAlongZAxis(rightHand, a);
        rotateSpecialPointAlongZAxis(leftElbow, a);
        rotateSpecialPointAlongZAxis(leftHand, a);

    }
    void changeAngle(float a) {
        a = angle + a;
        angle = a;
        rotateSpecialPointAlongZAxis(head, a);
        rotateSpecialPointAlongZAxis(butt, a);
        rotateSpecialPointAlongZAxis(rightKnee, a);
        rotateSpecialPointAlongZAxis(rightFoot, a);
        rotateSpecialPointAlongZAxis(leftKnee, a);
        rotateSpecialPointAlongZAxis(leftFoot, a);
        rotateSpecialPointAlongZAxis(rightElbow, a);
        rotateSpecialPointAlongZAxis(rightHand, a);
        rotateSpecialPointAlongZAxis(leftElbow, a);
        rotateSpecialPointAlongZAxis(leftHand, a);
    }


};

playerModel player = playerModel();



float cameraAngle = degreesToRadians(45);

float zRatio = sin(cameraAngle);

float yRatio = cos(cameraAngle);

void updateCameraAngle(float angle) {
    cameraAngle = degreesToRadians(angle);

    zRatio = sin(cameraAngle);
    yRatio = cos(cameraAngle);
}


point projectPoint(float x, float y, float z) {
    return point(x, y * yRatio - z * zRatio);
}





//put display size here when i add it --------------------------

int displayWidth = 2560;
int displayHeight = 1440;

int screenHeight = 180;
int screenWidth = 320;

GLFWwindow* window;

float ratio = (float)screenWidth / (float)displayWidth;

int centerX = screenWidth / 2;
int centerY = screenHeight / 2;

int textureWidth, textureHeight;

void drawRect(float x, float y, float width, float height) {
    glBegin(GL_QUADS);

    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);

    glEnd();
}
void drawSprite(unsigned int geepeeyouOne, int texturePath, float x, float y, float width, float height) {
    float tx, ty, tw, th;
    tx = calculatedTextureStorage[texturePath][0];

    ty = calculatedTextureStorage[texturePath][1];

    tw = calculatedTextureStorage[texturePath][2];

    th = calculatedTextureStorage[texturePath][3];


    glBindTexture(GL_TEXTURE_2D, geepeeyouOne);
    glBegin(GL_QUADS);

    glTexCoord2f(tx, ty);
    glVertex2f(x, y);

    glTexCoord2f(tx + tw, ty);
    glVertex2f(x + width, y);

    glTexCoord2f(tx + tw, ty + th);
    glVertex2f(x + width, y + height);

    glTexCoord2f(tx, ty + th);
    glVertex2f(x, y + height);

    glEnd(); //so proud :D
}
void drawEntireTexture(unsigned int geepeeyouone, float x, float y, float width, float height) {
    glBindTexture(GL_TEXTURE_2D, geepeeyouone);
    glBegin(GL_QUADS);

    glTexCoord2f(0, 1);
    glVertex2f(x, y);

    glTexCoord2f(1, 1);
    glVertex2f(x + width, y);

    glTexCoord2f(1, 0);
    glVertex2f(x + width, y + height);

    glTexCoord2f(0, 0);
    glVertex2f(x, y + height);

    glEnd(); //so proud :D
}


//so basicaly all the input variables go here okay pal?

double mouseX = 0, mouseY = 0;
double mouseRelX = 0, mouseRelY = 0;
double prevMouseX = 0, prevMouseY = 0;

//now all the lighting bites:

unsigned int textureColorBuffer;
unsigned int framebuffer;

void switchToFramebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, screenWidth, screenHeight);

    glClear(GL_COLOR_BUFFER_BIT);
}
void switchToScreen() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, displayWidth, displayHeight);
}

float flashlightAngle = 0;
float defaultDarkness = 0.9;
void circle(float r, float g, float b, float a, float x, float y, float radius, int segments = 32) {
    float angle = 0;
    float angleChange = (3.1416 * 2) / segments;


    glBegin(GL_TRIANGLE_FAN);

    glColor4f(r, g, b, a);
    glVertex2f(x, y);


    for (int i = 0; i <= segments; i++) {
        angle += angleChange;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
    glColor4f(1, 1, 1, 1);
}


void radialGradient(float r, float g, float b, float a, float innerr, float innerg, float innerb, float innera, float x, float y, float radius, int segments = 32) {
    float angle = 0;
    float angleChange = (3.1416 * 2) / segments;


    glBegin(GL_TRIANGLE_FAN);

    glColor4f(innerr, innerg, innerb, innera);
    glVertex2f(x, y);

    glColor4f(r, g, b, a);
    for (int i = 0; i <= segments; i++) {
        angle += angleChange;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
    glColor4f(1, 1, 1, 1);
}

void gradientRect(float r, float g, float b, float a, float innerr, float innerg, float innerb, float innera, point p1, point p2, point p3, point p4) {
    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);

    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);

    glColor4f(innerr, innerg, innerb, innera);

    glVertex2f(p3.x, p3.y);
    glVertex2f(p4.x, p4.y);

    glEnd();
    glColor4f(1, 1, 1, 1);
}

void radialGradientPolygon(float r, float g, float b, float a, float innerr, float innerg, float innerb, float innera, vector<point> points, float radius, bool moveWithPlayer = false) {
    float px, py;
    if (moveWithPlayer) {
        px = -playerX;
        py = -playerY;
    }
    else {
        px = 0;
        py = 0;
    }

    for (const point p : points) {

        radialGradient(r, g, b, a, innerr, innerg, innerb, innera, p.x + px, p.y + py, radius);


    }
    point segment1, segment2;
    point p1, p2, p3, p4;
    float angle;

    for (int i = 0; i < points.size(); i++) {
        if (i == 0) {
            segment2 = points.back();
        }
        else {
            segment2 = points[i - 1];
        }
        segment1 = points[i];
        angle = atan2(segment2.y - segment1.y, segment2.x - segment1.x);

        angle += 3.1416 / 2;

        p1.x = segment1.x + px + cos(angle) * radius;
        p1.y = segment1.y + py + sin(angle) * radius;

        p2.x = segment2.x + px + cos(angle) * radius;
        p2.y = segment2.y + py + sin(angle) * radius;

        p3.x = segment2.x + px;
        p3.y = segment2.y + py;

        p4.x = segment1.x + px;
        p4.y = segment1.y + py;

        gradientRect(r, g, b, a, innerr, innerg, innerb, innera, p1, p2, p3, p4);
    }
    glBegin(GL_POLYGON);

    glColor4f(innerr, innerg, innerb, innera);

    for (point p : points) {
        glVertex2f(p.x + px, p.y + py);
    }

    glEnd();

    glColor4f(1, 1, 1, 1);

}

void drawFlashlight(float x, float y, float angle, float widthAngle) {

    float angle1 = angle + widthAngle / 2;
    float angle2 = angle - widthAngle / 2;
    int dist = 150;
    int otherdist = 300;
    int blurRadius = 5;
    float brightness = 0;

    glBegin(GL_TRIANGLES);

    glColor4f(0, 0, 0, brightness);
    glVertex2f(x, y);
    glColor4f(0, 0, 0, defaultDarkness);
    glVertex2f(x + cos(angle1) * dist, y + sin(angle1) * dist);
    glVertex2f(x + cos(angle2) * dist, y + sin(angle2) * dist);

    glEnd();

    glBegin(GL_POLYGON);

    glColor4f(0, 0, 0, brightness);
    glVertex2f(x, y);
    glColor4f(0, 0, 0, defaultDarkness);
    glVertex2f(x + cos(angle1) * dist, y + sin(angle1) * dist);
    glVertex2f(x + cos(angle1 + widthAngle / 2) * dist, y + sin(angle1 + widthAngle / 2) * dist);
    glColor4f(0, 0, 0, defaultDarkness);
    glVertex2f(x + cos(angle1 + 3.1416 / 2) * blurRadius, y + sin(angle1 + 3.1416 / 2) * blurRadius);

    glEnd();

    glBegin(GL_POLYGON);

    glColor4f(0, 0, 0, brightness);
    glVertex2f(x, y);
    glColor4f(0, 0, 0, defaultDarkness);
    glVertex2f(x + cos(angle2) * dist, y + sin(angle2) * dist);
    glVertex2f(x + cos(angle2 - widthAngle / 2) * dist, y + sin(angle2 - widthAngle / 2) * dist);
    glColor4f(0, 0, 0, defaultDarkness);
    glVertex2f(x + cos(angle2 - 3.1416 / 2) * blurRadius, y + sin(angle2 - 3.1416 / 2) * blurRadius);

    glEnd();


    glColor4f(1, 1, 1, 1);

    radialGradient(0, 0, 0, defaultDarkness, 0, 0, 0, brightness, x, y, blurRadius);

}

//now the platyer bites

void drawPlayerPoint(specialPoint p) {
    point projected = projectPoint(p.rx, p.ry, p.rz);
    drawSprite(texture, __STAR__, centerX - 3 + projected.x, centerY - 3 + projected.y, 6, 6);
}

void drawPlayer(playerModel p) {
    drawPlayerPoint(p.head);
    drawPlayerPoint(p.butt);
    drawPlayerPoint(p.leftKnee);
    drawPlayerPoint(p.rightKnee);
    drawPlayerPoint(p.leftFoot);
    drawPlayerPoint(p.rightFoot);
    drawPlayerPoint(p.leftElbow);
    drawPlayerPoint(p.rightElbow);
    drawPlayerPoint(p.leftHand);
    drawPlayerPoint(p.rightHand);

}


// so basiclay now i put all the game logic variables :) ------------------

float cursorAngle = 0;

//so bascialy now all the game logic functions :) -----------------------

struct colisionData {
    bool colision = false;
    float yMovement = 0;
    float xMovement = 0;
    colisionData() {};
};


//moves the player around this crazy world :)
void movePlayer() {
    //float angle = atan2(mouseY * ratio - centerY, mouseX * ratio - centerX);
    float forward = glfwGetKey(window, GLFW_KEY_W) - glfwGetKey(window, GLFW_KEY_S);
    float sideways = glfwGetKey(window, GLFW_KEY_A) - glfwGetKey(window, GLFW_KEY_D);
    if (forward != 0 and sideways != 0) {
        forward *= 0.71;
        sideways *= 0.71;

    }
    if (forward < 0) {
        forward *= 0.8;
    }
    sideways *= 0.8;
    playerX += cos(cursorAngle) * forward + cos(cursorAngle - 3.1416 / 2) * sideways;
    playerY += yRatio * (sin(cursorAngle) * forward + sin(cursorAngle - 3.1416 / 2) * sideways);


}

//basic game opbject
struct sprite {
public:
    int x;
    int y;
    int textureID;
    int width;
    int height;
    sprite(int Xpos, int yPos, int textureNumber) {
        x = Xpos;
        y = yPos;
        textureID = textureNumber;
        width = getTextureWidth(textureID);
        height = getTextureHeight(textureID);
    }
    sprite() {}
};
//a radial light source that makes it less dark
struct radialLight {
    float r, g, b, a;
    float radius;
    float x, y;
    radialLight(float x, float y, float radius, float r = 0, float g = 0, float b = 0, float a = 0) : x(x), y(y), radius(radius), r(r), g(g), b(b), a(a) {}
};
//a polygonal light source for windows and doors with a builtin radial gradient system to avoid bluring
struct polygonLight {
    vector<point> points;
    float r, g, b, a;
    float radius;
    polygonLight(vector<point> points, float radius, float r = 0, float g = 0, float b = 0, float a = 0) : points(points), radius(radius), r(r), g(g), b(b), a(a) {}
};
//a recatngle hitbox for player colisions
struct rect {
    float x;
    float y;
    float width;
    float height;
};
//stores one map of sprites and lights
struct level {
public:
    vector<sprite> sprites;
    vector<radialLight> radialLights;
    vector<polygonLight> polygonLights;
    vector<rect> hitbox;
    level(vector<sprite> spriteVector = {}, vector<radialLight> radialLightVector = {}, vector<polygonLight> polygonLightVector = {}, vector<rect> hitboxes = {}) {
        sprites = spriteVector;
        radialLights = radialLightVector;
        polygonLights = polygonLightVector;
        hitbox = hitboxes;
    }

};



//HITBOX BYTES

//checks player colision with a single rectangle and returns fix if colision is true
colisionData checkColision(rect box) {
    float boxX, boxY, boxW, boxH;
    float pX, pY;
    colisionData output;

    pX = centerX + playerHitboxOffsetX;
    pY = centerY + playerHitboxOffsetY;

    boxX = box.x - playerX - playerRadius;
    boxY = box.y - playerY - playerRadius;
    boxW = box.width + playerRadius * 2;
    boxH = box.height + playerRadius * 2;

    if (
        pX > boxX
        and
        pY > boxY
        and
        pX < boxX + boxW
        and
        pY < boxY + boxH
        ) {

        output.colision = true;

        float x1, x2, y1, y2;

        x1 = boxX - pX;
        x2 = boxX + boxW - pX;
        y1 = boxY - pY;
        y2 = boxY + boxH - pY;

        if (abs(x2) < abs(x1)) {
            x1 = x2;
        }
        if (abs(y2) < abs(y1)) {
            y1 = y2;
        }
        if (abs(y1) < abs(x1)) {
            output.yMovement = y1;
        }
        else {
            output.xMovement = x1;
        }
    }
    else {
        output.colision = false;
    }
    return output;

}
colisionData OLDcheckColision(rect box) {
    colisionData output;
    float playerHitboxX = playerX + playerHitboxOffsetX;
    float playerHitboxY = playerY + playerHitboxOffsetY; //close but not right
    if (
        playerHitboxX > box.x - playerRadius
        and
        playerHitboxY > box.y - playerRadius
        and
        playerHitboxX < box.x + box.width + playerRadius
        and
        playerHitboxY < box.y + box.height + playerRadius
        ) {
        output.colision = true;
        float xChange, yChange;
        if (abs(box.x - playerRadius - playerHitboxX) < abs(box.x + box.width + playerRadius - playerHitboxX)) {
            xChange = box.x - playerRadius - playerHitboxX;
        }
        else {
            xChange = box.x + box.width + playerRadius - playerHitboxX;
        }
        if (abs(box.y - playerRadius - playerHitboxY) < abs(box.y + box.height + playerRadius - playerHitboxY)) {
            yChange = box.y - playerRadius - playerHitboxY;
        }
        else {
            yChange = box.y + box.height + playerRadius - playerHitboxY;
        }

        if (abs(yChange) < abs(xChange)) {
            output.yMovement = yChange;
        }
        else {
            output.xMovement = xChange;
        }
    }
    else {
        output.colision = false;
    }
    return output;
}
//calculates all player Colisions for the map
void playerColisions(level map) {
    for (int i = 0; i < 1; i++) {
        int count = 0;
        for (rect box : map.hitbox) {
            colisionData treat = checkColision(box);
            count += treat.colision;
            if (treat.colision and false) {
                cout << "player colided with rect at position X:" << playerX << ", Y:" << playerY << "and moved by  X:" << treat.xMovement << ", Y:" << treat.yMovement;
            }
            playerX += treat.xMovement;
            playerY += treat.yMovement;

        }
        if (count == 0) {
            break;
        }
    }
}

//DRAWING BITES

//draws the ground :D
void drawGround(int xPos, int yPos) {
    int offsetX, offsetY = 0;
    offsetX = -xPos % 200;
    offsetY = -yPos % 200;
    //cout << offsetY <<  '\n';
    for (int x = -1; x < 5; x++) {
        for (int y = -1; y < 3; y++) {
            drawSprite(texture, __DIRT__, x * 200 + offsetX, y * 200 + offsetY, 200, 200);
        }
    }
}
//dr4aws the hitboxes of a level
void drawAllHitboxes(level map) {
    glColor4f(1, 0.3, 0.6, 0.5);
    for (rect box : map.hitbox) {

        drawRect(box.x - playerX, box.y - playerY, box.width, box.height);
    }
    glColor4f(1, 1, 1, 1);
}
//draws a level
void drawMap(level map) {
    for (sprite obj : map.sprites) {
        drawSprite(texture, obj.textureID, obj.x - playerX, obj.y - playerY, obj.width, obj.height);
    }
    if (drawHitboxes) {
        drawAllHitboxes(map);
    }

}
//draws circle light scources of the a map
void drawRadialLights(level map) {
    for (radialLight light : map.radialLights) {
        radialGradient(0, 0, 0, defaultDarkness, light.r, light.g, light.b, light.a, light.x - playerX, light.y - playerY, light.radius);
    }
}
//draws polygon light sources of a mnap,  NEEDS TO HAPPEN WITH DEFAUT COLOR MIXING SETTINGS AND SHOULDNT BE ABLE TO OVERLAP WITH OTHER POLYGONS BUT CAN
void drawPolygonLights(level map) {
    for (polygonLight l : map.polygonLights) {
        radialGradientPolygon(0, 0, 0, defaultDarkness, l.r, l.g, l.b, l.a, l.points, l.radius, true);
    }
}
//holds the entire game level data 
struct storage {
    vector<level> levels;
    storage() {

    }

    void addLevel(vector<sprite> sprites = {}, vector<radialLight> radialLights = {}, vector<polygonLight> polygonLights = {}, vector<rect> hitbox = {}) {
        levels.push_back(level(sprites, radialLights, polygonLights, hitbox));
    }

};

//right before run make all the levells and stiore them all in ram cuz its efishent :D

//initialize

storage game;







//the big one :|
int main()
{
    //creates level one V
    game.addLevel(
        //Sprite List V
        {
            sprite(59, -32, __BUILDING__),
            sprite(217, -28, __BUILDING__),
            sprite(369, -31, __BUILDING__),

            sprite(171, 214, __PLAYER__)
        },
        //Radial Light Source List V
        {
            radialLight(177, 231, 113, 0, 0, 0, 0),
            radialLight(425, 190, 58, 0, 0, 0, 0),
            radialLight(575, 282, 88, 0, 0, 0, 0),
            radialLight(-68, 264, 49, 0, 0, 0, 0)
        },
        //Polygon Light Source List V
        {
        },
        //Hitbox List V
        {
            rect(90, -20, 136, 184),
            rect(249, -15, 135, 183),
            rect(401, -17, 137, 181)
        }
    );

    //game login functions V






    //BOring bites :(
    if (!glfwInit()) {
        cerr << "glfw didnt initialize becuase you are bad :(";
        return -1;
    }
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(displayWidth, displayHeight, "this is THEE best game of 1463", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }





    //-----------------------------THE-----------------------------------------------BIG----------------------------------------------------ONE :)
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    GLint maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

    std::cout << "Maximum texture size: "
        << maxTextureSize << " x "
        << maxTextureSize << std::endl;



    // heh
    //more boring treats from the store:
    glEnable(GL_TEXTURE_2D);

    glViewport(0, 0, displayWidth, displayHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(
        0, 640,      // world width
        360, 0,      // world height
        -1, 1
    );

    glMatrixMode(GL_MODELVIEW);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(
        0, screenWidth,
        screenHeight, 0,
        -1, 1
    );

    glMatrixMode(GL_MODELVIEW);

    //so basicialy only use one image cuz i dont want to type this all again :/
    //i decided to hid it cus its scary

    if (true) {
        int width, height, channels;
        unsigned char* pixels = stbi_load(
            "sprites.png",
            &width,
            &height,
            &channels,
            4
        );

        if (pixels)
        {
            cout << "so absicaly what ahppened is the iomage was sucesfulyt scaned by meta ai adn chatgpt and then forcxefuly shoved down the gee pee yous throught hole :)! " << width << "x" << height << endl; //get rid of slop
        }
        else
        {
            cout << "Failed\n";
        }

        textureWidth = width;
        textureHeight = height;



        glGenTextures(1, &texture);

        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            pixels
        );


    }
    //too much work for a picture bro :[

    calculatedTextureStorage = doTheTextureBites(textureStorage, textureWidth, textureHeight); //this gota go after the masive import i think


    //lighting texture stuff:

    //create a framebuffer bite :(

    //to hide it all
    if (true) {

        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);



        glGenTextures(1, &textureColorBuffer);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);



        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            screenWidth,
            screenHeight,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            NULL
        );

        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            textureColorBuffer,
            0
        );

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    while (!glfwWindowShouldClose(window) and !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwPollEvents();
        //get all the user inputs:
        //mabe divide by three idk yet
        glfwGetCursorPos(window, &mouseX, &mouseY);

        mouseRelX = mouseX - prevMouseX;
        mouseRelY = mouseY - prevMouseY;
        prevMouseX = mouseX;
        prevMouseY = mouseY;

        cursorAngle += mouseRelX / 1000;



        //flashlightAngle = atan2(mouseY*ratio - centerY, mouseX * ratio - centerX);
        flashlightAngle = cursorAngle;
        //all done

        //game Logic now 

        movePlayer();

        playerColisions(game.levels[0]);








        secretPlayerX = playerX;
        secretPlayerY = playerY;

        playerX = round(playerX);
        playerY = round(playerY);

        //draw the main world________________________
        switchToScreen();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawGround(playerX, playerY);

        drawMap(game.levels[0]);



        //draw the lighting effects________________________

        switchToFramebuffer();

        //fill the layer with light
        glClearColor(0, 0, 0, defaultDarkness);
        glClear(GL_COLOR_BUFFER_BIT);

        //put before blending change on purpose
        drawPolygonLights(game.levels[0]);

        //switch to defualt lighting blending
        glEnable(GL_BLEND);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ONE);
        glBlendEquationSeparate(GL_FUNC_ADD, GL_MIN);

        radialGradient(0, 0, 0, defaultDarkness, 0, 0, 0, 0.2, centerX, centerY, 70);
        point right = projectPoint(player.rightHand.rx, player.rightHand.ry, player.rightHand.rz);
        drawFlashlight(centerX + right.x, centerY + right.y, flashlightAngle, 0.4);

        drawRadialLights(game.levels[0]);

        //switch back to world bledning


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



        switchToScreen();





        drawEntireTexture(textureColorBuffer, 0, 0, screenWidth, screenHeight);


        cout << "HELLO" << "\n";


        //radialGradient(0, 0, 0, 0, 0, 0, 0, 1, centerX, centerY, 4);
        //circle(0, 0, 0, 1, centerX, centerY, 3);
        //drawSprite(texture, __PLAYER__, centerX - 8, centerY - 16, 16, 32);
        drawSprite(texture, __STAR__, centerX - 3 + cos(cursorAngle) * 50, centerY - 3 + sin(cursorAngle) * 50, 6, 6);

        //updateCameraAngle(clamp(mouseY/1440*90, 0.0, 90.0));

        player.motion.step();
        player.motion.apply();

        player.reset();
        player.setAngle(flashlightAngle + 3.1416 / 2);

        drawPlayer(player);





        glfwSwapBuffers(window);

        playerX = secretPlayerX;
        playerY = secretPlayerY;

    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

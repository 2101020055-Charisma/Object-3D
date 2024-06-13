

#include <GL/freeglut.h>
#include <iostream>
#include <math.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
float xrot = 0;
float yrot = 0;
float zrot = 0;
float xpos = 0;
float ypos = 0;
float zpos = 0;

bool mouseDown = false;
int lastMouseX, lastMouseY;


// Deklarasi ID tekstur
GLuint lantaiTID;
GLuint dindingTID;
GLuint kayu1TID;
GLuint kayu2TID;
GLuint kayu3TID;
GLuint kasurTID;
GLuint displayTID;
GLuint keyboardTID;
GLuint sampul1TID;
GLuint sampul2TID;
GLuint sampul3TID;
GLuint sampul4TID;


void loadTexture(const char* filename, GLuint &textureID) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    } else {
        printf("Failed to load texture: %s\n", filename);
    }
}



void myinit(void){
	glClearColor(0.2, 0.2, 0.2, 1.0);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_MODELVIEW);
	glPointSize(10.0);
	glLineWidth(7.0f);
	glEnable(GL_TEXTURE_2D); 
	loadTexture("lantai.jpg", lantaiTID);
	loadTexture("dinding.jpg", dindingTID);
	loadTexture("kayu1.jpg", kayu1TID);
	loadTexture("kayu2.jpg", kayu2TID);
	loadTexture("kayu3.jpg", kayu3TID);
	loadTexture("kasur.jpg", kasurTID);
	loadTexture("display.jpg", displayTID);
	loadTexture("keyboard.jpg", keyboardTID);
	loadTexture("sampul1.jpg", sampul1TID);
	loadTexture("sampul2.jpg", sampul2TID);
	loadTexture("sampul3.jpg", sampul3TID);
	loadTexture("sampul4.jpg", sampul4TID);
	
	
	
}

void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Definisikan parameter cahaya
    GLfloat lightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };   // Meningkatkan intensitas ambient
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };   // Meningkatkan intensitas diffuse
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // Intensitas specular tetap

    // Posisi cahaya di tengah ruangan
    GLfloat lightPosition[] = { 0.0f, 60.0f, 80.0f, 1.0f };

    // Parameter pelemahan cahaya
    GLfloat constantAttenuation = 0.5f;
    GLfloat linearAttenuation = 0.0001f;   // Mengurangi pelemahan linear
    GLfloat quadraticAttenuation = 0.00001f; // Mengurangi pelemahan kuadratik

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, constantAttenuation);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linearAttenuation);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);

    // Mengaktifkan properti material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Menetapkan parameter material
    GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat materialShininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);
}




void drawCube(float x, float y, float z, float sizeX, float sizeY, float sizeZ, GLfloat color[3], GLuint textureID = 0) {
    if (textureID != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
    } else {
        glDisable(GL_TEXTURE_2D);
    }

    glPushMatrix();
    

    GLfloat cubeVertices[8][3] =
    {
        {x, y, z},
        {x, y, z + sizeZ},
        {x + sizeX, y, z + sizeZ},
        {x + sizeX, y, z},
        {x, y + sizeY, z},
        {x, y + sizeY, z + sizeZ},
        {x + sizeX, y + sizeY, z + sizeZ},
        {x + sizeX, y + sizeY, z}
    };

    GLubyte quadIndices[6][4] =
    {
        {0, 1, 2, 3},
        {4, 5, 6, 7}, 
        {5, 1, 2, 6}, 
        {0, 4, 7, 3}, 
        {2, 3, 7, 6}, 
        {1, 5, 4, 0} 
    };

    GLfloat texCoords[4][2] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f}
    };

    glBegin(GL_QUADS);
    for (int i = 0; i < 6; i++) {
        glColor3fv(color);
        for (int j = 0; j < 4; j++) {
            if (textureID != 0) {
                glTexCoord2fv(texCoords[j]);
            }
            glVertex3fv(cubeVertices[quadIndices[i][j]]);
        }
    }
    glEnd();

    glColor3f(0.3f, 0.3f, 0.3f);
    glLineWidth(1.5f);
    for (int i = 0; i < 6; i++) {
        glBegin(GL_LINE_LOOP);
        for (int j = 0; j < 4; j++) {
            glVertex3fv(cubeVertices[quadIndices[i][j]]);
        }
        glEnd();
    }

    glPopMatrix();
}



// Fungsi untuk menggambar tabung oval tanpa garis di sisi-sisinya
void drawPillow(float x, float y, float z, float radiusA, float radiusB, float height, GLfloat color[3], float angleX, float angleY, float angleZ, int segments = 100) {
    GLfloat angle;
    GLfloat vertices[segments][2];

    
    for (int i = 0; i < segments; i++) {
        angle = 2.0f * M_PI * i / segments;
        vertices[i][0] = radiusA * cos(angle);
        vertices[i][1] = radiusB * sin(angle);
    }

   
    glPushMatrix();

   
    glTranslatef(x, y, z);    
    glRotatef(angleX, 1, 0, 0); 
    glRotatef(angleY, 0, 1, 0);
    glRotatef(angleZ, 0, 0, 1); 

    // Draw the bottom oval
    glBegin(GL_POLYGON);
    glColor3fv(color);
    for (int i = 0; i < segments; i++) {
        glVertex3f(vertices[i][0], vertices[i][1], 0);
    }
    glEnd();

    // Draw the top oval
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++) {
        glVertex3f(vertices[i][0], vertices[i][1], height);
    }
    glEnd();

    // Draw the sides
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; i++) {
        int index = i % segments;
        glVertex3f(vertices[index][0], vertices[index][1], 0);
        glVertex3f(vertices[index][0], vertices[index][1], height);
    }
    glEnd();
    glPopMatrix();
}







// Fungsi untuk menggambar cincin dengan posisi dan ukuran tertentu
void drawTorus(float x, float y, float z, float innerRadius, float outerRadius, GLfloat color[3])
{
    glPushMatrix();
    glColor3fv(color);
    glTranslatef(x, y, z);
    glutSolidTorus(innerRadius, outerRadius, 50, 50); // inner radius, outer radius, sides, rings
    glPopMatrix();
}

// Fungsi untuk menggambar alas jam
void drawBase(float x, float y, float z, float radius, int segments, GLfloat color[3])
{
    glColor3fv(color);
    glTranslatef(x, y, z);
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; ++i)
    {
        float theta = 2.0f * M_PI * i / segments;
        glVertex3f(x + radius * cos(theta), y + radius * sin(theta), z);
    }
    glEnd();
}


// Fungsi untuk menggambar jarum jam
void drawHand(float length, float width, GLfloat color[3]){
    glColor3fv(color);
    glBegin(GL_QUADS);
    glVertex2f(-width / 2, 0);
    glVertex2f(width / 2, 0);
    glVertex2f(width / 2, length);
    glVertex2f(-width / 2, length);
    glEnd();
}

// Fungsi untuk menggambar tanda jam
void drawMarks()
{
    for (int i = 0; i < 12; ++i)
    {
        float angle = 2.0f * M_PI * i / 12.0f;
        float x1 = cos(angle) * 0.8f;
        float y1 = sin(angle) * 0.8f;
        float x2 = cos(angle) * 0.9f;
        float y2 = sin(angle) * 0.9f;

        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }
}

// Fungsi untuk menggambar jam pada posisi tertentu
void drawClock(float x, float y, float z)
{
    // Simpan kondisi matriks saat ini
    glPushMatrix();
    glTranslatef(x, y, z);
    GLfloat ringColor[3] = {0.30196, 0.30196, 0.30196};
    GLfloat handColor[3] = {0.0f, 0.0f, 0.0f};
	GLfloat baseColor[3] = {1.00000, 1.00000, 1.00000};
    // Gambar cincin jam
    drawTorus(0.0f, 0.0f, 0.0f, 1.0f, 10.0f, ringColor);

	// Gambar alas jam (lingkaran datar)
	drawBase(0.0f, 0.0f, -0.1f, 10.0f, 100, baseColor);

    // Gambar tanda-tanda jam
    glColor3f(0.0f, 0.0f, 0.0f); // Set color to black for marks
    drawMarks();

    // Dapatkan waktu saat ini
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // Gambar jarum jam
    glPushMatrix();
    glRotatef(-(t->tm_hour % 12 + t->tm_min / 60.0f) * 30.0f, 0.0f, 0.0f, 1.0f);
    drawHand(5.0f, 1.0f, handColor);
    glPopMatrix();

    // Gambar jarum menit
    glPushMatrix();
    glRotatef(-t->tm_min * 6.0f, 0.0f, 0.0f, 1.0f);
    drawHand(7.0f, 1.0f, handColor);
    glPopMatrix();

    // Gambar jarum detik
    glPushMatrix();
    glRotatef(-t->tm_sec * 6.0f, 0.0f, 0.0f, 1.0f);
    drawHand(8.0f, 0.8f, handColor);
    glPopMatrix();

    glPopMatrix();
}


// Fungsi untuk menggambar bola 
void drawSphere(float x, float y, float z, float radius, GLfloat color[3]){
    glPushMatrix();
    glColor3fv(color);
    glTranslatef(x, y, z);
    glutSolidSphere(radius, 50, 50); 
    glPopMatrix();
}



	// warna
	GLfloat woodColor[3] = {0.67451, 0.45098, 0.22353};
	GLfloat lemari[3] = {0.77647, 0.54902, 0.32549};
	GLfloat lantai[3] = {0.5f, 0.5f, 0.5f};
	GLfloat dinding[3] = {0.00000, 0.40000, 0.40000};
	GLfloat putih[3] = {0.90196, 0.90196, 0.90196};
	GLfloat biru[3] = {0.14902, 0.30196, 0.45098}; 
    GLfloat coklatabu[3] = {0.41961, 0.41961, 0.27843};  
    GLfloat abugelap[3] = {0.18039, 0.18039, 0.12157}; 
    GLfloat merahgelap[3] = {0.60000, 0.00000, 0.00000}; 
	GLfloat kulit[3] = {1.00000, 0.83137, 0.50196};
	GLfloat birugelap[3] = {0.00000, 0.14902, 0.30196};
	GLfloat oren[3] = {0.80000, 0.32157, 0.00000};
	GLfloat hijau[3] = {0.00000, 0.40000, 0.00000};
	GLfloat hitam[3] = {0.10196, 0.10196, 0.10196};
	
	

    
	void gambardinding() {
    drawCube(-100.0f, -20.0f, -80.0f, 200.0f, -5.0f, 180.0f, lantai, lantaiTID);
    drawCube(-100.0f, -25.0f, -80.0f, 200.0f, 150.0f, -5.0f, dinding, dindingTID);
    drawCube(100.0f, -25.0f, -85.0f, 5.0f, 150.0f, 185.0f, dinding, dindingTID);
}




   void gambarkasur(){
    drawCube(-50.0f, -15.0f, -70.0f, 50.0f, 5.0f, 100.0f, woodColor, kayu1TID); // bagian ranjang bawah
    drawCube(-50.0f, -20.0f, -80.0f, 50.0f, 30.0f, 10.0f, woodColor, kayu1TID ); // bagian ranjang kepala
    drawCube(-50.0f, -20.0f, 30.0f, 50.0f, 30.0f, 10.0f, woodColor, kayu1TID); // bagian ranjang kaki
    drawCube(-53.0f, -10.0f, -70.0f, 56.0f, 8.0f, 100.0f, putih, kasurTID);
	drawPillow(-10.0f, 1.0f, -65.0f, 10.0f, 3.0f, 15.0f, biru, 0.0f, 0.0f, 0.0f); 
	drawPillow(-40.0f, 10.0f, -68.0f, 10.0f, 3.0f, 15.0f, biru, 60.0f, 0.0f, 0.0f); 
}


void gambarlemari() {
    drawCube(65.0f, -20.0f, 20.0f, 32.0f, 3.0f, 50.0f, woodColor, kayu3TID ); // Bagian bawah lemari
    drawCube(65.0f, 70.0f, 20.0f, 35.0f, -3.0f, 50.0f, woodColor, kayu3TID );  // Bagian atas lemari   
    drawCube(100.0f, -20.0f, 20.0f, -3.0f, 90.0f, 50.0f, woodColor, kayu3TID ); // Bagian kanan lemari
    drawCube(65.0f, -20.0f, 70.0f, 35.0f, 90.0f, 3.0f, woodColor, kayu3TID); // Bagian depan lemari
    drawCube(65.0f, -20.0f, 20.0f, 35.0f, 90.0f, -3.0f, woodColor, kayu3TID); // Bagian belakang lemari
    drawCube(65.0f, 10.0f, 20.0f, 32.0f, 3.0f, 50.0f, woodColor, kayu3TID); // Bagian lemari kecil 1
    drawCube(65.0f, -5.0f, 20.0f, 32.0f, 3.0f, 50.0f, woodColor, kayu3TID); // Bagian lemari kecil 2
    drawCube(65.0f, -20.0f, 43.0f, 32.0f, 87.0f, 3.0f,  woodColor, kayu3TID); // Bagian tengah lemari
	drawCube(68.0f, -17.0f, 20.0f, 30.0f, 85.0f, 50.0f, lemari, kayu1TID); //  lemari 
 	drawSphere(66.0f, 40.0f, 38.0f, 2.0f, abugelap); //kiri
   	drawSphere(66.0f, 40.0f, 50.0f, 2.0f, abugelap); // Kanan
   
   	
}

void gambarrak(){
	drawCube(65.0f, 65.0f, -75.0f, 35.0f, 5.0f, 78.0f, woodColor, kayu1TID); // atas meja
    drawCube(66.0f, 71.0f, -65.0f, 8.0f, 14.0f, 0.2f, putih, sampul1TID);//1
    drawCube(66.0f, 71.0f, -63.0f, 8.0f, 14.0f, 0.2f, abugelap);
    drawCube(74.0f, 71.0f, -65.0f, 0.3f, 14.0f, 2.0f, abugelap);
	drawCube(66.5f, 71.0f, -64.5f, 7.0f, 14.0f, 1.5f, putih);
	drawCube(66.0f, 71.0f, -61.0f, 8.0f, 14.0f, 0.2f, kulit, sampul2TID);//2
    drawCube(66.0f, 71.0f, -59.0f, 8.0f, 14.0f, 0.2f, hitam);
    drawCube(74.0f, 71.0f, -61.0f, 0.3f, 14.0f, 2.0f, hitam);
	drawCube(66.5f, 71.0f, -60.5f, 7.0f, 14.0f, 1.5f, putih);
	drawCube(66.0f, 71.0f, -57.0f, 8.0f, 14.0f, 0.2f, putih, sampul4TID);//3
    drawCube(66.0f, 71.0f, -55.0f, 8.0f, 14.0f, 0.2f, birugelap);
    drawCube(74.0f, 71.0f, -57.0f, 0.3f, 14.0f, 2.0f, abugelap);
	drawCube(66.5f, 71.0f, -56.5f, 7.0f, 14.0f, 1.5f, putih);
	drawCube(66.0f, 71.0f, -52.0f, 8.0f, 0.2f, 14.0f, oren);//4
	drawCube(66.0f, 73.0f, -52.0f, 8.0f, 0.2f, 14.0f, putih, sampul3TID);
	drawCube(74.0f, 71.0f, -52.0f, 0.3f, 2.0f, 14.0f, birugelap);
	drawCube(66.5f, 71.5f, -52.0f, 7.0f, 1.5f, 14.0f, putih);
	

}

void gambarmeja(){

    drawCube(65.0f, -20.0f, -75.0f, 35.0f, 30.0f, 3.0f, woodColor, kayu2TID); //kaki kiri
	drawCube(65.0f, -20.0f, 0.0f, 35.0f, 30.0f, 3.0f, woodColor, kayu2TID); // kaki kanan lemari
	drawCube(65.0f, -20.0f, -30.0f, 35.0f, 30.0f, 3.0f, woodColor, kayu2TID); // kaki kiri lemari
	drawCube(65.0f, 10.0f, -75.0f, 35.0f, 5.0f, 78.0f, woodColor, kayu3TID); // atas meja
	drawCube(96.0f, -5.0f, -75.0f, 3.0f, 15.0f, 78.0f, woodColor, kayu2TID); // belakang meja
	drawCube(65.0f, -15.0f, -30.0f, 35.0f, 3.0f, 30.0f, woodColor, kayu2TID); // bawah lemari 
	drawCube(68.0f, -12.0f, -27.0f, 30.0f, 25.0f, 27.0f, lemari, kayu1TID); //  lemari 
	drawSphere(66.0f, 0.0f, -5.0f, 2.0f, woodColor); // gagang lemari
	
	drawCube(75.0f, 16.0f, -62.0f, 8.0f, 2.0f, 14.0f, woodColor, keyboardTID);//keybord
	drawCube(70.0f, 16.0f, -65.0f, 15.0f, 2.0f, 3.0f, coklatabu);//kiri	
	drawCube(70.0f, 16.0f, -45.0f, 15.0f, 2.0f, -3.0f, coklatabu);//kanan
	drawCube(70.0f, 16.0f, -62.0f, 5.0f, 2.0f, 14.0f, coklatabu);//bawah
	drawCube(85.0f, 16.0f, -62.0f, -2.0f, 2.0f, 14.0f, coklatabu);//atas
	
	
	drawCube(85.0f, 20.0f, -63.0f, 2.0f, 14.0f, 16.0f, putih, displayTID);//laptop
	drawCube(85.0f, 17.0f, -65.0f, 2.0f, 20.0f, 2.0f, coklatabu);//kiri
	drawCube(85.0f, 17.0f, -45.0f, 2.0f, 20.0f, -2.0f, coklatabu);//kanan
	drawCube(85.0f, 17.0f, -63.0f, 2.0f, 3.0f, 16.0f, coklatabu);//bawah
	drawCube(85.0f, 37.0f, -63.0f, 2.0f, -3.0f, 16.0f, coklatabu);//atas

}

void gambarjam(){
	drawClock(20.0f, 70.0f, -78.0f);
	drawCube(40.0f, -1.0f, -65.0f, 20.0f, 3.0f, 20.0f, putih, kayu1TID); //kursi
	drawCube(40.0f, -20.0f, -65.0f, 2.0f, 40.0f, 2.0f, woodColor, kayu1TID); //kaki
	drawCube(58.0f, -20.0f, -65.0f, 2.0f, 19.0f, 2.0f, woodColor, kayu1TID); //kaki
	drawCube(58.0f, -20.0f, -47.0f, 2.0f, 19.0f, 2.0f, woodColor, kayu1TID); //kaki
	drawCube(40.0f, -20.0f, -47.0f, 2.0f, 40.0f, 2.0f, woodColor, kayu1TID); //kaki
	drawCube(42.0f, 10.0f, -68.0f, 2.0f, 15.0f, 27.0f, putih, kayu1TID); //atas
}

void ukur(int lebar, int tinggi){
	if (tinggi==0) tinggi=1;
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,lebar/tinggi, 5, 450);
	glTranslatef(0, 0, -300);
	glMatrixMode(GL_MODELVIEW);
	
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseDown = true;
            lastMouseX = x;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            mouseDown = false;
        }
    }
}

// Mouse motion event handler
void mousemotion(int x, int y) {
    if (mouseDown) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;
        
        xrot += (float) dy * 0.1f;
        yrot += (float) dx * 0.1f;
        
        lastMouseX = x;
        lastMouseY = y;
        
        glutPostRedisplay(); // Mark the current window as needing to be redisplayed
    }
}




void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
        case 'W':
            xrot -= 3.0;
            break;
        case 's':
        case 'S':
            xrot += 3.0;
            break;
        case 'd':
        case 'D':
            yrot += 3.0;
            break;
        case 'a':
        case 'A':
            yrot -= 3.0;
            break;
        case 'e':
        case 'E':
            zrot -= 3.0;
            break;
        case 'f':
        case 'F':
            zrot += 3.0;
            break;
        case 'j':
        case 'J':
            xpos += 3.0;
            break;
        case 'l':
        case 'L':
            xpos -= 3.0;
            break;
        case 'i':
        case 'I':
            zpos += 3.0;
            break;
        case 'k':
        case 'K':
            zpos -= 3.0;
            break;
        case 'h':
        case 'H':
            ypos -= 3.0;
            break;
        case 'u':
        case 'U':
            ypos += 3.0;
            break;
    }
    glutPostRedisplay();
}




void tampilan (void){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0,0,3,0,0,0,0,1,0);
	 glTranslatef(xpos, ypos, zpos);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);
    gambardinding();
	gambarlemari();
	gambarkasur();
	gambarmeja();
	gambarrak();
	gambarjam();
	glutSwapBuffers();
}



int main (int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(240, 80);
	glutInitWindowSize(750, 600);
	glutCreateWindow("Rumah 3D");
	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(tampilan);
	glutReshapeFunc(ukur);
	glutMouseFunc(mouse);
	glutMotionFunc(mousemotion);
	glutKeyboardFunc(keyboard);
	myinit();
	initLighting();
	glutMainLoop();

}



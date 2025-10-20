#include "Scene4.h"

CScene4::CScene4()
{
	pCamera = NULL;
	pTexto = NULL;
	pTextures = NULL;
	
	bIsWireframe = false;
	bIsCameraFPS = true;

	iFPS = 0;
	iFrames = 0;
	ulLastFPS = 0;
	szTitle[256] = 0;

	// Cria gerenciador de impressão de texto na tela
	pTexto = new CTexto();

	// Cria camera
	pCamera = new CCamera(0.0f, 1.0f, 20.0f);

	// Cria o Timer
	pTimer = new CTimer();
	pTimer->Init();

	fTimerPosY = 0.0f;
	fRenderPosY = 0.0f;

	// Carrega todas as texturas
	pTextures = new CTexture();	
	pTextures->CreateTextureMipMap(0, "../Scene1/TriangleFaces.bmp");


	LightAmbient[0] = 1.0f;		LightAmbient[1] = 1.0f;		LightAmbient[2] = 1.0f;	LightAmbient[3] = 1.0f;
	LightDiffuse[0] = 1.0f;		LightDiffuse[1] = 1.0f;		LightDiffuse[2] = 1.0f;		LightDiffuse[3] = 1.0f;
	LightSpecular[0] = 1.0f;	LightSpecular[1] = 1.0f;	LightSpecular[2] = 1.0f;	LightSpecular[3] = 1.0f;
	LightPosition[0] = 0.0f;	LightPosition[1] = 10.0f;	LightPosition[2] = 20.0f;	LightPosition[3] = 1.0f;
	//LightDirection[0] = 0.0f;	LightDirection[1] = -1.0f;	LightDirection[2] = 0.0f;

	
	fLightVelocity = 0.5f;
}


CScene4::~CScene4(void)
{
	if (pTexto)
	{
		delete pTexto;
		pTexto = NULL;
	}

	if (pTextures)
	{
		delete pTextures;
		pTextures = NULL;
	}

	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pTimer)
	{
		delete pTimer;
		pTimer = NULL;
	}	
}




int CScene4::DrawGLScene(void)	// Função que desenha a cena
{
	// Get FPS
	if (GetTickCount() - ulLastFPS >= 1000)	// When A Second Has Passed...
	{
		ulLastFPS = GetTickCount();				// Update Our Time Variable
		iFPS = iFrames;							// Save The FPS
		iFrames = 0;							// Reset The FPS Counter
	}
	iFrames++;									// FPS counter
	
	pTimer->Update();							// Atualiza o timer

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Limpa a tela e o Depth Buffer
	glLoadIdentity();									// Inicializa a Modelview Matrix Atual


	// Seta as posições da câmera
	pCamera->setView();

	

	// Desenha os eixos do sistema cartesiano
	DrawAxis();

	// Modo FILL ou WIREFRAME (pressione barra de espaço)	
	if (bIsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (INÍCIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	// Desenha a Lâmpada
	glPushMatrix();
	glTranslatef(LightPosition[0], LightPosition[1], LightPosition[2]);
	auxSolidSphere(0.5);
	glPopMatrix();

	

	// Habilita iluminação global (PHONG)
	glEnable(GL_LIGHTING);

	// Habilita a fonte de luz (lâmpada 0)
	glEnable(GL_LIGHT0);

	// Atribuindo os valores para os componentes de definição da lâmpada
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);



	// Aplicando Material de reflexão aos objetos
	MatAmbient[0] = 0.0f;	MatAmbient[1] = 0.0f;	MatAmbient[2] = 0.0f;	MatAmbient[3] = 1.0f;
	MatDiffuse[0] = 0.0f;	MatDiffuse[1] = 0.25f;	MatDiffuse[2] = 0.0f;	MatDiffuse[3] = 1.0f;
	MatSpecular[0] = 0.0f;	MatSpecular[1] = 0.0f;	MatSpecular[2] = 0.0f;	MatSpecular[3] = 1.0f;
	MatShininess = 0.0f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);
	
	// Desenha grid 
	Draw3DSGrid(50.0f, 50.0f);






	// Aplicando Material de reflexão aos objetos
	MatAmbient[0] = 0.2125f;	MatAmbient[1] = 0.1275f;	MatAmbient[2] = 0.054f;	MatAmbient[3] = 1.0f;
	MatDiffuse[0] = 0.714f;	MatDiffuse[1] = 0.4284f;	MatDiffuse[2] = 0.18144f;	MatDiffuse[3] = 1.0f;
	MatSpecular[0] = 0.393548f;	MatSpecular[1] = 0.271906f;	MatSpecular[2] = 0.166721f;	MatSpecular[3] = 1.0f;
	MatShininess = 25.6f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);

	// Desenha um Teapot de Bronze
	glPushMatrix();
	glTranslatef(-10.0f, 4.0f, 0.0f);
	glutSolidTeapot(5.0);
	glPopMatrix();


	// Aplicando Material de reflexão aos objetos
	MatAmbient[0] = 0.24725f;	MatAmbient[1] = 0.1995f;	MatAmbient[2] = 0.0745f;	MatAmbient[3] = 1.0f;
	MatDiffuse[0] = 0.75164f;	MatDiffuse[1] = 0.60648f;	MatDiffuse[2] = 0.22648f;	MatDiffuse[3] = 1.0f;
	MatSpecular[0] = 0.628281f;	MatSpecular[1] = 0.555802f;	MatSpecular[2] = 0.366065f;	MatSpecular[3] = 1.0f;
	MatShininess = 51.2f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);

	// Desenha um cone de ouro
	glPushMatrix();
	glTranslatef(10.0f, 0.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glutSolidCone(5.0, 10.0, 10, 10);
	glPopMatrix();


	glEnable(GL_TEXTURE_2D);

	// Aplicando Material de reflexão aos objetos
	MatAmbient[0] = 0.0f;	MatAmbient[1] = 0.0f;	MatAmbient[2] = 0.0f;	MatAmbient[3] = 1.0f;
	MatDiffuse[0] = 1.0f;	MatDiffuse[1] = 1.0f;	MatDiffuse[2] = 1.0f;	MatDiffuse[3] = 1.0f;
	MatSpecular[0] = 0.0f;	MatSpecular[1] = 0.0f;	MatSpecular[2] = 0.0f;	MatSpecular[3] = 1.0f;
	MatShininess = 0.0f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);

	// Desenha uma Pirâmide
	glPushMatrix();
	DrawPiramid(0.0f, 5.0f, 10.0f, 5.0, 0);	
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);








	// Desabilita iluminação global (PHONG)
	glDisable(GL_LIGHT0);

	// Desabilita iluminação global (PHONG)
	glDisable(GL_LIGHTING);









	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	fTimerPosY = pTimer->GetTime() / 1000.0f;
	fRenderPosY += 1.0f;

	// Impressão de texto na tela...
	// Muda para modo de projeção ortogonal 2D
	// OBS: Desabilite Texturas e Iluminação antes de entrar nesse modo de projeção
	OrthoMode(0, 0, WIDTH, HEIGHT);


	glPushMatrix();
	glTranslatef(0.0f, HEIGHT - 100, 0.0f);	// Move 1 unidade para dentro da tela (eixo Z)

	// Cor da fonte
	glColor3f(1.0f, 1.0f, 0.0f);


	glRasterPos2f(10.0f, 0.0f);	// Posicionando o texto na tela
	if (!bIsWireframe) {
		pTexto->glPrint("[TAB]  Modo LINE"); // Imprime texto na tela
	}
	else {
		pTexto->glPrint("[TAB]  Modo FILL");
	}


	//// Camera LookAt
	glRasterPos2f(10.0f, 40.0f);
	pTexto->glPrint("Player LookAt  : %f, %f, %f", pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);

	//// Posição do Player
	glRasterPos2f(10.0f, 60.0f);
	pTexto->glPrint("Player Position: %f, %f, %f", pCamera->Position[0], pCamera->Position[1], pCamera->Position[2]);

	//// Imprime o FPS da aplicação e o Timer
	glRasterPos2f(10.0f, 80.0f);
	pTexto->glPrint("Frames-per-Second: %d ---- Timer: %.1f segundos", iFPS, (pTimer->GetTime()/1000));


	glPopMatrix();

	// Muda para modo de projeção perspectiva 3D
	PerspectiveMode();

	return true;
}




void CScene4::MouseMove(void) // Tratamento de movimento do mouse
{
	// Realiza os cálculos de rotação da visão do Player (através das coordenadas
	// X do mouse.
	POINT mousePos;
	int middleX = WIDTH >> 1;
	int middleY = HEIGHT >> 1;

	GetCursorPos(&mousePos);

	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	SetCursorPos(middleX, middleY);

	fDeltaX = (float)((middleX - mousePos.x)) / 10;
	fDeltaY = (float)((middleY - mousePos.y)) / 10;

	// Rotaciona apenas a câmera
	pCamera->rotateGlob(-fDeltaX, 0.0f, 1.0f, 0.0f);
	pCamera->rotateLoc(-fDeltaY, 1.0f, 0.0f, 0.0f);
}

void CScene4::KeyPressed(void) // Tratamento de teclas pressionadas
{

	// Verifica se a tecla 'W' foi pressionada e move o Player para frente
	if (GetKeyState('W') & 0x80)
	{
		pCamera->moveGlob(pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);
	}
	// Verifica se a tecla 'S' foi pressionada e move o Player para tras
	else if (GetKeyState('S') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Forward[0], -pCamera->Forward[1], -pCamera->Forward[2]);
	}
	// Verifica se a tecla 'A' foi pressionada e move o Player para esquerda
	else if (GetKeyState('A') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Right[0], -pCamera->Right[1], -pCamera->Right[2]);
	}
	// Verifica se a tecla 'D' foi pressionada e move o Player para direira
	else if (GetKeyState('D') & 0x80)
	{
		pCamera->moveGlob(pCamera->Right[0], pCamera->Right[1], pCamera->Right[2]);
	}
	else if (GetKeyState('Q') & 0x80)
	{
		pCamera->moveGlob(0.0f, -pCamera->Up[1], 0.0f);
	}
	else if (GetKeyState('E') & 0x80)
	{
		pCamera->moveGlob(0.0f, pCamera->Up[1], 0.0f);
	}
	// Senão, interrompe movimento do Player
	else
	{
	}

	if (GetKeyState(VK_UP) & 0x80)
	{
		LightPosition[2] -= fLightVelocity;
	}
	if (GetKeyState(VK_DOWN) & 0x80)
	{
		LightPosition[2] += fLightVelocity;
	}
	if (GetKeyState(VK_LEFT) & 0x80)
	{
		LightPosition[0] -= fLightVelocity;
	}
	if (GetKeyState(VK_RIGHT) & 0x80)
	{
		LightPosition[0] += fLightVelocity;
	}
	if (GetKeyState(VK_PRIOR) & 0x80)
	{
		LightPosition[1] += fLightVelocity;
	}
	if (GetKeyState(VK_NEXT) & 0x80)
	{
		LightPosition[1] -= fLightVelocity;
	}

}

void CScene4::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
{
	switch (wParam)
	{
	case VK_TAB:
		bIsWireframe = !bIsWireframe;
		break;

	case VK_SPACE:
		pTimer->Init();
		break;

	case VK_RETURN:
		break;

	}

}

//	Cria um grid horizontal ao longo dos eixos X e Z
void CScene4::Draw3DSGrid(float width, float length)
{

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.3f, 0.0f);
	glPushMatrix();
	for (float i = -width; i <= length; i += 1)
	{
		for (float j = -width; j <= length; j += 1)
		{
			// inicia o desenho das linhas
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(i, 0.0f, j + 1);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 1, 0.0f, j + 1);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(i + 1, 0.0f, j);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(i, 0.0f, j);
			glEnd();
		}
	}
	glPopMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



void CScene4::DrawAxis()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	// Eixo X
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);

	// Eixo Y
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	glVertex3f(0.0f, -1000.0f, 0.0f);

	// Eixo Z
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glVertex3f(0.0f, 0.0f, -1000.0f);
	glEnd();
	glPopMatrix();
}


void CScene4::DrawPiramid(float x, float y, float z, float size, int texID)
{

	pTextures->ApplyTexture(texID);	// Aplica textura do trianglo
	glPushMatrix();
	glTranslatef(x, y, z);	// Move a pirâmide para a posição (x, y, z)


	// Desenha uma pirâmide
	glBegin(GL_TRIANGLES);

	// Base
	glVertex3f(-size, -size, -size);
	glVertex3f(size, -size, -size);
	glVertex3f(size, -size, size);
	glVertex3f(-size, -size, -size);
	glVertex3f(size, -size, size);
	glVertex3f(-size, -size, size);

	// Faces laterais
	glm::vec3 normal = CalculateTriangleNormalVector(
		glm::vec3(-size, -size, -size),
		glm::vec3(0.0f, size, 0.0f),
		glm::vec3(size, -size, -size)
	);
	glNormal3f(normal.x, normal.y, normal.z);
	glTexCoord2f(0.02f, 0.1f); glVertex3f(-size, -size, -size);
	glTexCoord2f(0.23f, 0.45f); glVertex3f(0.0f, size, 0.0f);
	glTexCoord2f(0.435f, 0.1f); glVertex3f(size, -size, -size);




	normal = CalculateTriangleNormalVector(
		glm::vec3(size, -size, -size),
		glm::vec3(0.0f, size, 0.0f),
		glm::vec3(size, -size, size)
	);
	glNormal3f(normal.x, normal.y, normal.z);
	glTexCoord2f(0.52f, 0.1f); glVertex3f(size, -size, -size);
	glTexCoord2f(0.73f, 0.45f); glVertex3f(0.0f, size, 0.0f);
	glTexCoord2f(0.94f, 0.1f); glVertex3f(size, -size, size);

	normal = CalculateTriangleNormalVector(
		glm::vec3(size, -size, size),
		glm::vec3(0.0f, size, 0.0f),
		glm::vec3(-size, -size, size)
	);
	glNormal3f(normal.x, normal.y, normal.z);
	glTexCoord2f(0.02f, 0.56f); glVertex3f(size, -size, size);
	glTexCoord2f(0.23f, 0.9f); glVertex3f(0.0f, size, 0.0f);
	glTexCoord2f(0.435f, 0.56f); glVertex3f(-size, -size, size);

	normal = CalculateTriangleNormalVector(
		glm::vec3(-size, -size, size),
		glm::vec3(0.0f, size, 0.0f),
		glm::vec3(-size, -size, -size)
	);
	glNormal3f(normal.x, normal.y, normal.z);
	glTexCoord2f(0.53f, 0.56f); glVertex3f(-size, -size, size);
	glTexCoord2f(0.73f, 0.9f); glVertex3f(0.0f, size, 0.0f);
	glTexCoord2f(0.93f, 0.56f); glVertex3f(-size, -size, -size);
	glEnd();
	glPopMatrix();


}


glm::vec3 CScene4::CalculateTriangleNormalVector(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
	/*
			   v3
			   /\
			  /  \
			 /    \
			/      \
			+------+
		   v1      v2
	*/

	glm::vec3 A, B;
	A.x = v2.x - v1.x;
	A.y = v2.y - v1.y;
	A.z = v2.z - v1.z;

	B.x = v3.x - v1.x;
	B.y = v3.y - v1.y;
	B.z = v3.z - v1.z;

	// Calcula o Cross Product
	glm::vec3 normal;
	normal.x = A.y * B.z - A.z * B.y;
	normal.y = A.z * B.x - A.x * B.z;
	normal.z = A.x * B.y - A.y * B.x;

	// Calcula a magnitude do vetor normal
	double magnitude = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

	// Normaliza o vetor normal
	glm::vec3 normalizedVector;
	normalizedVector.x = (normal.x / magnitude);
	normalizedVector.y = (normal.y / magnitude);
	normalizedVector.z = (normal.z / magnitude);

	return normalizedVector;
}


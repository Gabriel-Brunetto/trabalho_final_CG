#include "Scene3.h"

CScene3::CScene3()
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
	pCamera = new CCamera(0.0f, 1.0f, 20.0f, 0.5f);

	// Cria o Timer
	pTimer = new CTimer();
	pTimer->Init();

	fTimerPosY = 0.0f;
	fRenderPosY = 0.0f;

	// Carrega todas as texturas
	pTextures = new CTexture();	
	pTextures->CreateTextureAnisotropic(0, "../Scene1/Crate.bmp");
	pTextures->CreateTextureAnisotropic(1, "../Scene1/grass.bmp");
	pTextures->CreateTextureAnisotropic(2, "../Scene1/B1.jpg");
	pTextures->CreateTextureAnisotropic(3, "../Scene1/TriangleFaces.bmp");
	pTextures->CreateTextureAnisotropic(4, "../Scene1/Blood_Spatter.bmp");
	pTextures->CreateTextureAnisotropic(5, "../Scene1/decal2.bmp");
	


	fX = 0.0f;
	fY = 0.0f;
	fZ = 0.0f;
	fVelocity = 0.1f;	// Velocidade de movimento do teapot
	fAngle = 0.0f;		// Ângulo de rotação do teapot
	fSize = 1.0f;		// Tamanho do teapot (raio)
	fAnim = 0.0f;

}


CScene3::~CScene3(void)
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




int CScene3::DrawGLScene(void)	// Função que desenha a cena
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

	// Desenha grid 
	//Draw3DSGrid(20.0f, 20.0f);

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

	glEnable(GL_TEXTURE_2D);				// Habilita o uso de texturas

	glPushAttrib(GL_TEXTURE_BIT);

	// Configura a primeira camada de textura
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	pTextures->ApplyTexture(1);	// Aplica a primeira camada de textura no chão
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);

	// Configura a segunda camada de textura
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	pTextures->ApplyTexture(5);	// Aplica a segunda camada de textura no chão
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

	// Incrementa animação da camada 1
	fAnim += 0.01;

	// Desenha o chão
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);	// Move o chão para a posição (x, y, z)
		glBegin(GL_QUADS);
		glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
		glMultiTexCoord2f(GL_TEXTURE1, 0.0f + fAnim, 0.0f);
		glVertex3f(-20.0f, 0.0f,  20.0f);
		
		glMultiTexCoord2f(GL_TEXTURE0, 3.0f, 0.0f);
		glMultiTexCoord2f(GL_TEXTURE1, 1.0f + fAnim, 0.0f);
		glVertex3f( 20.0f, 0.0f,  20.0f);

		glMultiTexCoord2f(GL_TEXTURE0, 3.0f, 3.0f);
		glMultiTexCoord2f(GL_TEXTURE1, 1.0f + fAnim, 1.0f);
		glVertex3f( 20.0f, 0.0f, -20.0f);

		glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 3.0f);
		glMultiTexCoord2f(GL_TEXTURE1, 0.0f + fAnim, 1.0f);
		glVertex3f(-20.0f, 0.0f, -20.0f);
		glEnd();
	glPopMatrix();

	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);




	

	// Desenha o muro
	// Configura a primeira camada de textura
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	pTextures->ApplyTexture(2);	// Aplica a primeira camada de textura no muro
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);

	// Configura a segunda camada de textura
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	pTextures->ApplyTexture(4);	// Aplica a segunda camada de textura no muro
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);


	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 30.0f);	// Move o chão para a posição (x, y, z)
		glBegin(GL_QUADS);
		glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
		glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
		glVertex3f(-50.0f,  0.0f, -50.0f);

		glMultiTexCoord2f(GL_TEXTURE0, 5.0f, 0.0f);
		glMultiTexCoord2f(GL_TEXTURE1, 5.0f, 0.0f);
		glVertex3f( 50.0f,  0.0f, -50.0f);

		glMultiTexCoord2f(GL_TEXTURE0, 5.0f, 1.0f);
		glMultiTexCoord2f(GL_TEXTURE1, 5.0f, 1.0f);
		glVertex3f( 50.0f, 20.0f, -50.0f);

		glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
		glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
		glVertex3f(-50.0f, 20.0f, -50.0f);
	glEnd();
	glPopMatrix();

	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	
	glPopAttrib();


	
	// Desenha a piramide
	DrawPiramid(-5.0f, 3.0f, 0.0f, 3.0f);

	//// Desenha um cubo no centro da cena
	DrawCube(5.0f, 3.0f, 0.0f);

	glDisable(GL_TEXTURE_2D);				// Desabilita o uso de texturas


	








	fAngle += 0.5f;	// Incrementa o ângulo de rotação do teapot
	if (fAngle >= 360.0f)	// Se o ângulo for maior que 360 graus, reinicia o ângulo
		fAngle = 0.0f;


	





	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	fTimerPosY = pTimer->GetTime() / 1000.0f;
	fRenderPosY += 0.2f;

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




void CScene3::MouseMove(void) // Tratamento de movimento do mouse
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

void CScene3::KeyPressed(void) // Tratamento de teclas pressionadas
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

	if (GetKeyState(VK_LEFT) & 0x80)
	{
		fX -= fVelocity;	
	}
	if (GetKeyState(VK_RIGHT) & 0x80)
	{
		fX += fVelocity;
	}
	if (GetKeyState(VK_UP) & 0x80)
	{
		fZ -= fVelocity;
	}
	if (GetKeyState(VK_DOWN) & 0x80)
	{
		fZ += fVelocity;
	}
	if (GetKeyState(VK_NEXT) & 0x80)
	{
		fY -= fVelocity;
	}
	if (GetKeyState(VK_PRIOR) & 0x80)
	{
		fY += fVelocity;
	}

	if (GetKeyState(VK_ADD) & 0x80)
	{
		fSize += 0.01f;
	}
	if (GetKeyState(VK_SUBTRACT) & 0x80)
	{
		fSize -= 0.01f;
		if (fSize < 0.01f) fSize = 0.01f; // Impede que o tamanho fique negativo
	}


}

void CScene3::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
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
void CScene3::Draw3DSGrid(float width, float length)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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



void CScene3::DrawAxis()
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


void CScene3::DrawCube(float x, float y, float z)
{
	pTextures->ApplyTexture(0);	// Aplica textura da caixa
	glPushMatrix();
	glTranslatef(x, y, z);	// Move o cubo para a posição (x, y, z)

	// Desenha um cubo no centro da cena
	glBegin(GL_QUADS);
	// Face frontal
	glColor3ub(181, 87, 222);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

	// Face traseira
	glColor3ub(101, 161, 79);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glColor3ub(181, 42, 172);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glColor3ub(121, 147, 181);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glColor3ub(212, 117, 93);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);

	// Face esquerda
	glColor3ub(255, 0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);

	// Face direita
	glColor3ub(0, 255, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);

	// Face superior
	glColor3ub(0, 0, 255);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);

	// Face inferior
	glColor3ub(255, 255, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();

	glPopMatrix();
}

void CScene3::DrawPiramid(float x, float y, float z, float size)
{

	pTextures->ApplyTexture(3);	// Aplica textura do trianglo
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
	glTexCoord2f(0.02f, 0.1f); glVertex3f(-size, -size, -size);
	glTexCoord2f(0.23f, 0.45f); glVertex3f(0.0f, size, 0.0f);
	glTexCoord2f(0.435f, 0.1f); glVertex3f(size, -size, -size);
	
	glTexCoord2f(0.52f, 0.1f); glVertex3f(size, -size, -size);
	glTexCoord2f(0.73f, 0.45f); glVertex3f(0.0f, size, 0.0f);
	glTexCoord2f(0.94f, 0.1f); glVertex3f(size, -size, size);

	glTexCoord2f(0.02f, 0.56f); glVertex3f(size, -size, size);
	glTexCoord2f(0.23f, 0.9f); glVertex3f(0.0f, size, 0.0f);
	glTexCoord2f(0.435f, 0.56f); glVertex3f(-size, -size, size);

	glTexCoord2f(0.53f, 0.56f); glVertex3f(-size, -size, size);
	glTexCoord2f(0.73f, 0.9f); glVertex3f(0.0f, size, 0.0f);
	glTexCoord2f(0.93f, 0.56f); glVertex3f(-size, -size, -size);
	glEnd();
	glPopMatrix();


}
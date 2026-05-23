/* Made by InuzDev ; AutoClicker CLI */

#include <stdio.h>
#include <windows.h>
#include <winuser.h>

#define DEFAULT_INTERVAL_MS 100 // 10 clicks per second
#define MIN_INTERVAL_MS 10      // 100 clicks per second max
#define MAX_INTERVAL_MS 5000    // 0.2 clicks per second min

static volatile int g_running = 0; // Clicking active?
static volatile int g_quit = 0;    // program exit flag
static volatile int g_interval = DEFAULT_INTERVAL_MS;

void clear_line();

void printStatus();
void sendClick();

DWORD WINAPI clickThread(LPVOID lpParam);

int main() {
  printf("Inu's Autoclicker\n");
  printf("Press F6 to start");

  printStatus();

  HANDLE thread = CreateThread(NULL, 0, clickThread, NULL, 0, NULL);

  if (!thread) {
    fprintf(stderr, "Failed to create click thread\n");
    return 1;
  }

  while (!g_quit) {
    if (GetAsyncKeyState(VK_F6) & 1) {
      g_running = !g_running;
      printStatus();
    }

    if (GetAsyncKeyState(VK_F7) & 1) {
      g_interval -= 10;

      if (g_interval < MIN_INTERVAL_MS) {
        g_interval = MIN_INTERVAL_MS;
        printStatus();
      }
    }

    if (GetAsyncKeyState(VK_F8) & 1) {
      g_interval += 10;
      if (g_interval > MAX_INTERVAL_MS) {
        g_interval = MAX_INTERVAL_MS;
      }
    }

    if (GetAsyncKeyState(VK_F9) & 1) {
      g_running = 0;
      g_quit = 1;
    }

    Sleep(10);
  }

  WaitForSingleObject(thread, 500);
  CloseHandle(thread);

  printf("Closing program. . .");
  return 0;
}

/*
 * Function: PrintStatus
 * Objective: Print a menu for the user to see in the terminal
 */
void printStatus() {
  clear_line();
  printf("[%s] interval: %dms (~%d cps) | F6 = toggle ; F7 = Less speed ; F8 = "
         "More speed ; F9 = Quit",
         g_running ? "ON " : "OFF", g_interval, 1600 / g_interval);

  fflush(stdout);
}

/*
 * Function: SendClick()
 * Objective: Send the automated input using WIN32 Struct.
 */
void sendClick() {
  INPUT input[2];

  // Zeroes the structure first, unitialized padding bytes can cause undefined
  // behavior.
  ZeroMemory(input, sizeof(input));

  /* Mouse down */

  input[0].type = INPUT_MOUSE;
  input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

  /* mouse up */
  input[1].type = INPUT_MOUSE;
  input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

  SendInput(2, input, sizeof(INPUT));
}

/*
 * Function: clickThread()
 * Objective: Create a seperate thread, that loops while the program is running;
 * To avoid issues with the CPU, we use a Sleep(10), if the autoclicker is off.
 * */
DWORD WINAPI clickThread(LPVOID lpParam) {
  (void)lpParam;
  while (!g_quit) {
    if (g_running) {
      sendClick();
      Sleep(g_interval);
    } else {
      Sleep(10);
    }
  }

  return 0;
}

/*
 * Function: clear_line()
 * Objective: Clear the line in the CLI application.
 * */
void clear_line() { printf("\r                          \r"); }

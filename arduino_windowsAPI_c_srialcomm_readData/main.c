#include <windows.h>
#include <stdio.h>
#include <string.h>

HANDLE COMM_PORT_HANDLE;
char COMM_PORT_NAME[] = "\\\\.\\COM13";
BOOL STATUS;

DCB DCB_PARAMS = {0};

void readData();

int CHOICE;

int main()
{

    COMM_PORT_HANDLE = CreateFile(COMM_PORT_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (COMM_PORT_HANDLE == INVALID_HANDLE_VALUE)
    {
        printf("Error in opening serial port");
        return 1;
    }
    else
    {
        printf("opening serial port successful");
        printf("\n");

        STATUS = FlushFileBuffers(COMM_PORT_HANDLE);
        if (!STATUS)
        {
            printf("Failed to flush serial port");
            CloseHandle(COMM_PORT_HANDLE);
        }
        printf("reading data ...");
        printf("\n");
        while (1)
        {
            readData();
        }
    }
}

void readData()
{

    DWORD DW_EVENT_MASK;
    char IN_COMMING_CHAR;
    char R_BUFFER[256];
    DWORD NO_OF_BYTES;

    STATUS = GetCommState(COMM_PORT_HANDLE, &DCB_PARAMS);

    DCB_PARAMS.DCBlength = sizeof(DCB);
    DCB_PARAMS.BaudRate = CBR_9600;
    DCB_PARAMS.ByteSize = 8;
    DCB_PARAMS.Parity = NOPARITY;
    DCB_PARAMS.StopBits = ONESTOPBIT;

    STATUS = SetCommState(COMM_PORT_HANDLE, &DCB_PARAMS);

    COMMTIMEOUTS timeouts = {0};

    timeouts.ReadIntervalTimeout = 0;
    timeouts.ReadTotalTimeoutConstant = 100;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 100;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    if (SetCommTimeouts(COMM_PORT_HANDLE, &timeouts) == FALSE)
    {
        return;
    }

    STATUS = SetCommMask(COMM_PORT_HANDLE, EV_RXCHAR);
    STATUS = WaitCommEvent(COMM_PORT_HANDLE, &DW_EVENT_MASK, NULL);

    int i = 0;
    do
    {
        STATUS = ReadFile(COMM_PORT_HANDLE, &IN_COMMING_CHAR, sizeof(IN_COMMING_CHAR), &NO_OF_BYTES, NULL);
        R_BUFFER[i] = IN_COMMING_CHAR;
        i++;
    } while (NO_OF_BYTES > 0);

    int j = 0;
    for (j = 0; j < i - 1; j++)
    {
        printf("%c", R_BUFFER[j]);
    }
    //CloseHandle(COMM_PORT_HANDLE);
}
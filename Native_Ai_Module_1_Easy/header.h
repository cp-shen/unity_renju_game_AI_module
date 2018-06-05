#pragma once

# define _DLLExport __declspec (dllexport) //标记为导出函数;  

extern "C" void _DLLExport drive_ai(const char *json_in, char *json_out, int out_capacity);

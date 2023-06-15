#include <stdio.h>
#include <string.h>

int rolling_xor_file(const char* src_file_path, const char* dst_file_path, const char* xor_str) {
    FILE* srcFile = fopen(src_file_path, "rb");
    if (srcFile == NULL) {
        fprintf(stderr, "Could not open file: %s\n", src_file_path);
        return 1;
    }

    FILE* dstFile = fopen(dst_file_path, "wb");
    if (dstFile == NULL) {
        fprintf(stderr, "Could not create file: %s\n", dst_file_path);
        return 1;
    }

    /////////////////////////////////

    size_t xor_str_len = strlen(xor_str);
    if (xor_str_len == 0) {
        fprintf(stderr, "XOR string must not be empty.\n");
        fclose(srcFile);
        return 1;
    }

    int c;
    size_t xor_str_idx = 0;
    while ((c = fgetc(srcFile)) != EOF) {
        // Compute XOR and move file position pointer back
        char xor_c = c ^ xor_str[xor_str_idx];

        // Write XOR'd char back to file
        if (fputc(xor_c, dstFile) == EOF) {
            fprintf(stderr, "Could not write to file: %s\n", src_file_path);
            fclose(srcFile);
            fclose(dstFile);
            return 1;
        }

        xor_str_idx = (xor_str_idx + 1) % xor_str_len;
    }

    fclose(srcFile);
    fclose(dstFile);
    return 0;
}

int main() {
    
    // Important! all the following file paths must NOT include Hebrew or any other language other than English.
    
    // The Encrypted file path:
    const char* src_file_path = "C:\\Users\\Admin\\Downloads\\Algo_OS_Exam_080623_enc.zip";
    
    // Where do you want to put the Decrypted file:
    const char* dst_file_path = "C:\\Users\\Admin\\Downloads\\Algo_OS_Exam_080623.zip";
    
    // The password:
    const char* xor_str = "Secret!";

    if (rolling_xor_file(src_file_path, dst_file_path, xor_str) != 0) {
        fprintf(stderr, "Rolling XOR operation failed.\n");
        return 1;
    }

    printf("Rolling XOR operation succeeded.\n");
    return 0;
}
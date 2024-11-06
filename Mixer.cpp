#include <iostream>
#include <string>
using namespace std;

class Cipher {
public:
    virtual string encode(const string& plaintext) = 0;
    virtual string decode(const string& ciphertext) = 0;

    virtual ~Cipher() = default;
};

class CaesarCipher : public Cipher {
private:
    int offset;

public:
    CaesarCipher(int offset) {
        this->offset = offset;
    }

    string encode(const string& input) override {
        string output = "";
        for (char c : input) {
            if (isalpha(c)) {
                char base = islower(c) ? 'a' : 'A';
                output += char(int(base + (c - base + offset) % 26));
            } else {
                output += c;
            }
        }
        return output;
    }

    string decode(const string& input) override {
        string output = "";
        for (char c : input) {
            if (isalpha(c)) {
                char base = islower(c) ? 'a' : 'A';
                output += char(int(base + (c - base - offset + 26) % 26));
            } else {
                output += c;
            }
        }
        return output;
    }

    ~CaesarCipher() override {}
};

// Base class for compression
class Compressor {
public:
    virtual string compress(const string& plainText) = 0;
    virtual string decompress(const string& compressedText) = 0;

    virtual ~Compressor() = default;
};

// Run-Length Encoding (RLE) Compression
class RLECompressor : public Compressor {
public:
    RLECompressor() {}
    ~RLECompressor() override {}

    string compress(const string& plainText) override {
        string compressed = "";
        int length = plainText.length();

        for (int i = 0; i < length; ++i) {
            char currentChar = plainText[i];
            int count = 1;

            while (i + 1 < length && plainText[i] == plainText[i + 1]) {
                count++;
                i++;
            }

            while (count > 255) {
                compressed += currentChar;
                compressed += (char)255;
                count -= 255;
            }

            compressed += currentChar;
            compressed += (char)count;
        }

        return compressed;
    }

    string decompress(const string& compressedText) override {
        string decompressed = "";
        int length = compressedText.length();

        for (int i = 0; i < length; i += 2) {
            char currentChar = compressedText[i];
            int count = (unsigned char)compressedText[i + 1];

            decompressed.append(count, currentChar);
        }

        return decompressed;
    }
};

// Mixed Cipher class to combine Caesar and RLE
class MixedCipher {
private:
    CaesarCipher caesar;
    RLECompressor rle;

public:

    MixedCipher(int caesarShift) : caesar(caesarShift) {}

    string mixEncode(const string& input) {
        string encodedText = caesar.encode(input);
        string compressedText = rle.compress(encodedText); 
        return compressedText;
    }

    // Mix decoding: RLE Decompression -> Caesar Decryption
    string mixDecode(const string& input) {
        string decompressedText = rle.decompress(input);  // First, apply RLE Decompression
        string decodedText = caesar.decode(decompressedText);  // Then, apply Caesar Cipher Decryption
        return decodedText;
    }
};

// Main program to test mixed encoding and decoding
int main() {
    string inputText;
    int shiftValue;

    // Input the text and shift value for Caesar Cipher
    cout << "Enter text: ";
    getline(cin, inputText);

    cout << "Enter shift value for Caesar Cipher: ";
    cin >> shiftValue;

    MixedCipher mixedCipher(shiftValue);

    // Perform mixed encoding (Caesar + RLE)
    string encodedText = mixedCipher.mixEncode(inputText);
    cout << "Mixed Encoded text (Caesar + RLE): " << encodedText << endl;

    // Perform mixed decoding (RLE + Caesar)
    string decodedText = mixedCipher.mixDecode(encodedText);
    cout << "Mixed Decoded text (RLE + Caesar): " << decodedText << endl;

    return 0;
}

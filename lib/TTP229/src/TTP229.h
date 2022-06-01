/*
 * MIT License
 *
 * Copyright (c) 2019-2020 Erriez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef TTP229_H_
#define TTP229_H_

/*!
 * \brief ErriezTTP229 class
 */
#define BUTTON_1 1
#define BUTTON_2 2
#define BUTTON_3 3
#define BUTTON_UP 4
#define BUTTON_5 5
#define BUTTON_6 6
#define BUTTON_7 7
#define BUTTON_DOWN 8
#define BUTTON_9 9
#define BUTTON_10 10
#define BUTTON_11 11
#define BUTTON_MENU 12
#define BUTTON_DELETE 13
#define BUTTON_14 14
#define BUTTON_BACK 15
#define BUTTON_ENT 16

class TTP229
{
public:
    void begin(uint8_t sclPin, uint8_t sdoPin);
    uint8_t GetKey16();

    volatile bool keyChange;
    
private:
    bool GetBit();
        
    uint8_t _sclPin;
    uint8_t _sdoPin;
};

#endif /* ERRIEZ_TTP229_H_ */
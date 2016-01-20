# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef UVMSC_BOOST_PREPROCESSOR_ARITHMETIC_DEC_HPP
# define UVMSC_BOOST_PREPROCESSOR_ARITHMETIC_DEC_HPP
#
# include <packages/boost/include/preprocessor/config/config.hpp>
#
# /* UVMSC_BOOST_PP_DEC */
#
# if ~UVMSC_BOOST_PP_CONFIG_FLAGS() & UVMSC_BOOST_PP_CONFIG_MWCC()
#    define UVMSC_BOOST_PP_DEC(x) UVMSC_BOOST_PP_DEC_I(x)
# else
#    define UVMSC_BOOST_PP_DEC(x) UVMSC_BOOST_PP_DEC_OO((x))
#    define UVMSC_BOOST_PP_DEC_OO(par) UVMSC_BOOST_PP_DEC_I ## par
# endif
#
# define UVMSC_BOOST_PP_DEC_I(x) UVMSC_BOOST_PP_DEC_ ## x
#
# define UVMSC_BOOST_PP_DEC_0 0
# define UVMSC_BOOST_PP_DEC_1 0
# define UVMSC_BOOST_PP_DEC_2 1
# define UVMSC_BOOST_PP_DEC_3 2
# define UVMSC_BOOST_PP_DEC_4 3
# define UVMSC_BOOST_PP_DEC_5 4
# define UVMSC_BOOST_PP_DEC_6 5
# define UVMSC_BOOST_PP_DEC_7 6
# define UVMSC_BOOST_PP_DEC_8 7
# define UVMSC_BOOST_PP_DEC_9 8
# define UVMSC_BOOST_PP_DEC_10 9
# define UVMSC_BOOST_PP_DEC_11 10
# define UVMSC_BOOST_PP_DEC_12 11
# define UVMSC_BOOST_PP_DEC_13 12
# define UVMSC_BOOST_PP_DEC_14 13
# define UVMSC_BOOST_PP_DEC_15 14
# define UVMSC_BOOST_PP_DEC_16 15
# define UVMSC_BOOST_PP_DEC_17 16
# define UVMSC_BOOST_PP_DEC_18 17
# define UVMSC_BOOST_PP_DEC_19 18
# define UVMSC_BOOST_PP_DEC_20 19
# define UVMSC_BOOST_PP_DEC_21 20
# define UVMSC_BOOST_PP_DEC_22 21
# define UVMSC_BOOST_PP_DEC_23 22
# define UVMSC_BOOST_PP_DEC_24 23
# define UVMSC_BOOST_PP_DEC_25 24
# define UVMSC_BOOST_PP_DEC_26 25
# define UVMSC_BOOST_PP_DEC_27 26
# define UVMSC_BOOST_PP_DEC_28 27
# define UVMSC_BOOST_PP_DEC_29 28
# define UVMSC_BOOST_PP_DEC_30 29
# define UVMSC_BOOST_PP_DEC_31 30
# define UVMSC_BOOST_PP_DEC_32 31
# define UVMSC_BOOST_PP_DEC_33 32
# define UVMSC_BOOST_PP_DEC_34 33
# define UVMSC_BOOST_PP_DEC_35 34
# define UVMSC_BOOST_PP_DEC_36 35
# define UVMSC_BOOST_PP_DEC_37 36
# define UVMSC_BOOST_PP_DEC_38 37
# define UVMSC_BOOST_PP_DEC_39 38
# define UVMSC_BOOST_PP_DEC_40 39
# define UVMSC_BOOST_PP_DEC_41 40
# define UVMSC_BOOST_PP_DEC_42 41
# define UVMSC_BOOST_PP_DEC_43 42
# define UVMSC_BOOST_PP_DEC_44 43
# define UVMSC_BOOST_PP_DEC_45 44
# define UVMSC_BOOST_PP_DEC_46 45
# define UVMSC_BOOST_PP_DEC_47 46
# define UVMSC_BOOST_PP_DEC_48 47
# define UVMSC_BOOST_PP_DEC_49 48
# define UVMSC_BOOST_PP_DEC_50 49
# define UVMSC_BOOST_PP_DEC_51 50
# define UVMSC_BOOST_PP_DEC_52 51
# define UVMSC_BOOST_PP_DEC_53 52
# define UVMSC_BOOST_PP_DEC_54 53
# define UVMSC_BOOST_PP_DEC_55 54
# define UVMSC_BOOST_PP_DEC_56 55
# define UVMSC_BOOST_PP_DEC_57 56
# define UVMSC_BOOST_PP_DEC_58 57
# define UVMSC_BOOST_PP_DEC_59 58
# define UVMSC_BOOST_PP_DEC_60 59
# define UVMSC_BOOST_PP_DEC_61 60
# define UVMSC_BOOST_PP_DEC_62 61
# define UVMSC_BOOST_PP_DEC_63 62
# define UVMSC_BOOST_PP_DEC_64 63
# define UVMSC_BOOST_PP_DEC_65 64
# define UVMSC_BOOST_PP_DEC_66 65
# define UVMSC_BOOST_PP_DEC_67 66
# define UVMSC_BOOST_PP_DEC_68 67
# define UVMSC_BOOST_PP_DEC_69 68
# define UVMSC_BOOST_PP_DEC_70 69
# define UVMSC_BOOST_PP_DEC_71 70
# define UVMSC_BOOST_PP_DEC_72 71
# define UVMSC_BOOST_PP_DEC_73 72
# define UVMSC_BOOST_PP_DEC_74 73
# define UVMSC_BOOST_PP_DEC_75 74
# define UVMSC_BOOST_PP_DEC_76 75
# define UVMSC_BOOST_PP_DEC_77 76
# define UVMSC_BOOST_PP_DEC_78 77
# define UVMSC_BOOST_PP_DEC_79 78
# define UVMSC_BOOST_PP_DEC_80 79
# define UVMSC_BOOST_PP_DEC_81 80
# define UVMSC_BOOST_PP_DEC_82 81
# define UVMSC_BOOST_PP_DEC_83 82
# define UVMSC_BOOST_PP_DEC_84 83
# define UVMSC_BOOST_PP_DEC_85 84
# define UVMSC_BOOST_PP_DEC_86 85
# define UVMSC_BOOST_PP_DEC_87 86
# define UVMSC_BOOST_PP_DEC_88 87
# define UVMSC_BOOST_PP_DEC_89 88
# define UVMSC_BOOST_PP_DEC_90 89
# define UVMSC_BOOST_PP_DEC_91 90
# define UVMSC_BOOST_PP_DEC_92 91
# define UVMSC_BOOST_PP_DEC_93 92
# define UVMSC_BOOST_PP_DEC_94 93
# define UVMSC_BOOST_PP_DEC_95 94
# define UVMSC_BOOST_PP_DEC_96 95
# define UVMSC_BOOST_PP_DEC_97 96
# define UVMSC_BOOST_PP_DEC_98 97
# define UVMSC_BOOST_PP_DEC_99 98
# define UVMSC_BOOST_PP_DEC_100 99
# define UVMSC_BOOST_PP_DEC_101 100
# define UVMSC_BOOST_PP_DEC_102 101
# define UVMSC_BOOST_PP_DEC_103 102
# define UVMSC_BOOST_PP_DEC_104 103
# define UVMSC_BOOST_PP_DEC_105 104
# define UVMSC_BOOST_PP_DEC_106 105
# define UVMSC_BOOST_PP_DEC_107 106
# define UVMSC_BOOST_PP_DEC_108 107
# define UVMSC_BOOST_PP_DEC_109 108
# define UVMSC_BOOST_PP_DEC_110 109
# define UVMSC_BOOST_PP_DEC_111 110
# define UVMSC_BOOST_PP_DEC_112 111
# define UVMSC_BOOST_PP_DEC_113 112
# define UVMSC_BOOST_PP_DEC_114 113
# define UVMSC_BOOST_PP_DEC_115 114
# define UVMSC_BOOST_PP_DEC_116 115
# define UVMSC_BOOST_PP_DEC_117 116
# define UVMSC_BOOST_PP_DEC_118 117
# define UVMSC_BOOST_PP_DEC_119 118
# define UVMSC_BOOST_PP_DEC_120 119
# define UVMSC_BOOST_PP_DEC_121 120
# define UVMSC_BOOST_PP_DEC_122 121
# define UVMSC_BOOST_PP_DEC_123 122
# define UVMSC_BOOST_PP_DEC_124 123
# define UVMSC_BOOST_PP_DEC_125 124
# define UVMSC_BOOST_PP_DEC_126 125
# define UVMSC_BOOST_PP_DEC_127 126
# define UVMSC_BOOST_PP_DEC_128 127
# define UVMSC_BOOST_PP_DEC_129 128
# define UVMSC_BOOST_PP_DEC_130 129
# define UVMSC_BOOST_PP_DEC_131 130
# define UVMSC_BOOST_PP_DEC_132 131
# define UVMSC_BOOST_PP_DEC_133 132
# define UVMSC_BOOST_PP_DEC_134 133
# define UVMSC_BOOST_PP_DEC_135 134
# define UVMSC_BOOST_PP_DEC_136 135
# define UVMSC_BOOST_PP_DEC_137 136
# define UVMSC_BOOST_PP_DEC_138 137
# define UVMSC_BOOST_PP_DEC_139 138
# define UVMSC_BOOST_PP_DEC_140 139
# define UVMSC_BOOST_PP_DEC_141 140
# define UVMSC_BOOST_PP_DEC_142 141
# define UVMSC_BOOST_PP_DEC_143 142
# define UVMSC_BOOST_PP_DEC_144 143
# define UVMSC_BOOST_PP_DEC_145 144
# define UVMSC_BOOST_PP_DEC_146 145
# define UVMSC_BOOST_PP_DEC_147 146
# define UVMSC_BOOST_PP_DEC_148 147
# define UVMSC_BOOST_PP_DEC_149 148
# define UVMSC_BOOST_PP_DEC_150 149
# define UVMSC_BOOST_PP_DEC_151 150
# define UVMSC_BOOST_PP_DEC_152 151
# define UVMSC_BOOST_PP_DEC_153 152
# define UVMSC_BOOST_PP_DEC_154 153
# define UVMSC_BOOST_PP_DEC_155 154
# define UVMSC_BOOST_PP_DEC_156 155
# define UVMSC_BOOST_PP_DEC_157 156
# define UVMSC_BOOST_PP_DEC_158 157
# define UVMSC_BOOST_PP_DEC_159 158
# define UVMSC_BOOST_PP_DEC_160 159
# define UVMSC_BOOST_PP_DEC_161 160
# define UVMSC_BOOST_PP_DEC_162 161
# define UVMSC_BOOST_PP_DEC_163 162
# define UVMSC_BOOST_PP_DEC_164 163
# define UVMSC_BOOST_PP_DEC_165 164
# define UVMSC_BOOST_PP_DEC_166 165
# define UVMSC_BOOST_PP_DEC_167 166
# define UVMSC_BOOST_PP_DEC_168 167
# define UVMSC_BOOST_PP_DEC_169 168
# define UVMSC_BOOST_PP_DEC_170 169
# define UVMSC_BOOST_PP_DEC_171 170
# define UVMSC_BOOST_PP_DEC_172 171
# define UVMSC_BOOST_PP_DEC_173 172
# define UVMSC_BOOST_PP_DEC_174 173
# define UVMSC_BOOST_PP_DEC_175 174
# define UVMSC_BOOST_PP_DEC_176 175
# define UVMSC_BOOST_PP_DEC_177 176
# define UVMSC_BOOST_PP_DEC_178 177
# define UVMSC_BOOST_PP_DEC_179 178
# define UVMSC_BOOST_PP_DEC_180 179
# define UVMSC_BOOST_PP_DEC_181 180
# define UVMSC_BOOST_PP_DEC_182 181
# define UVMSC_BOOST_PP_DEC_183 182
# define UVMSC_BOOST_PP_DEC_184 183
# define UVMSC_BOOST_PP_DEC_185 184
# define UVMSC_BOOST_PP_DEC_186 185
# define UVMSC_BOOST_PP_DEC_187 186
# define UVMSC_BOOST_PP_DEC_188 187
# define UVMSC_BOOST_PP_DEC_189 188
# define UVMSC_BOOST_PP_DEC_190 189
# define UVMSC_BOOST_PP_DEC_191 190
# define UVMSC_BOOST_PP_DEC_192 191
# define UVMSC_BOOST_PP_DEC_193 192
# define UVMSC_BOOST_PP_DEC_194 193
# define UVMSC_BOOST_PP_DEC_195 194
# define UVMSC_BOOST_PP_DEC_196 195
# define UVMSC_BOOST_PP_DEC_197 196
# define UVMSC_BOOST_PP_DEC_198 197
# define UVMSC_BOOST_PP_DEC_199 198
# define UVMSC_BOOST_PP_DEC_200 199
# define UVMSC_BOOST_PP_DEC_201 200
# define UVMSC_BOOST_PP_DEC_202 201
# define UVMSC_BOOST_PP_DEC_203 202
# define UVMSC_BOOST_PP_DEC_204 203
# define UVMSC_BOOST_PP_DEC_205 204
# define UVMSC_BOOST_PP_DEC_206 205
# define UVMSC_BOOST_PP_DEC_207 206
# define UVMSC_BOOST_PP_DEC_208 207
# define UVMSC_BOOST_PP_DEC_209 208
# define UVMSC_BOOST_PP_DEC_210 209
# define UVMSC_BOOST_PP_DEC_211 210
# define UVMSC_BOOST_PP_DEC_212 211
# define UVMSC_BOOST_PP_DEC_213 212
# define UVMSC_BOOST_PP_DEC_214 213
# define UVMSC_BOOST_PP_DEC_215 214
# define UVMSC_BOOST_PP_DEC_216 215
# define UVMSC_BOOST_PP_DEC_217 216
# define UVMSC_BOOST_PP_DEC_218 217
# define UVMSC_BOOST_PP_DEC_219 218
# define UVMSC_BOOST_PP_DEC_220 219
# define UVMSC_BOOST_PP_DEC_221 220
# define UVMSC_BOOST_PP_DEC_222 221
# define UVMSC_BOOST_PP_DEC_223 222
# define UVMSC_BOOST_PP_DEC_224 223
# define UVMSC_BOOST_PP_DEC_225 224
# define UVMSC_BOOST_PP_DEC_226 225
# define UVMSC_BOOST_PP_DEC_227 226
# define UVMSC_BOOST_PP_DEC_228 227
# define UVMSC_BOOST_PP_DEC_229 228
# define UVMSC_BOOST_PP_DEC_230 229
# define UVMSC_BOOST_PP_DEC_231 230
# define UVMSC_BOOST_PP_DEC_232 231
# define UVMSC_BOOST_PP_DEC_233 232
# define UVMSC_BOOST_PP_DEC_234 233
# define UVMSC_BOOST_PP_DEC_235 234
# define UVMSC_BOOST_PP_DEC_236 235
# define UVMSC_BOOST_PP_DEC_237 236
# define UVMSC_BOOST_PP_DEC_238 237
# define UVMSC_BOOST_PP_DEC_239 238
# define UVMSC_BOOST_PP_DEC_240 239
# define UVMSC_BOOST_PP_DEC_241 240
# define UVMSC_BOOST_PP_DEC_242 241
# define UVMSC_BOOST_PP_DEC_243 242
# define UVMSC_BOOST_PP_DEC_244 243
# define UVMSC_BOOST_PP_DEC_245 244
# define UVMSC_BOOST_PP_DEC_246 245
# define UVMSC_BOOST_PP_DEC_247 246
# define UVMSC_BOOST_PP_DEC_248 247
# define UVMSC_BOOST_PP_DEC_249 248
# define UVMSC_BOOST_PP_DEC_250 249
# define UVMSC_BOOST_PP_DEC_251 250
# define UVMSC_BOOST_PP_DEC_252 251
# define UVMSC_BOOST_PP_DEC_253 252
# define UVMSC_BOOST_PP_DEC_254 253
# define UVMSC_BOOST_PP_DEC_255 254
# define UVMSC_BOOST_PP_DEC_256 255
#
# endif

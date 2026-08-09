// elf2x68k.cpp
// Python original by yunkya2

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <algorithm>

struct EHeader {
    uint8_t ei_cls;
    uint8_t ei_data;
    uint8_t ei_version;
    uint8_t ei_osabi;
    uint8_t ei_abiversion;
    uint16_t type;
    uint16_t machine;
    uint32_t version;
    uint32_t entry;
    uint32_t phoff;
    uint32_t shoff;
    uint32_t flags;
    uint16_t ehsize;
    uint16_t phentsize;
    uint16_t phnum;
    uint16_t shentsize;
    uint16_t shnum;
    uint16_t shstrndx;

    EHeader(std::ifstream& fh) {
        fh.seekg(0);
        char elfident[16];
        fh.read(elfident, 16);
        
        ei_cls = static_cast<uint8_t>(elfident[4]);
        ei_data = static_cast<uint8_t>(elfident[5]);
        ei_version = static_cast<uint8_t>(elfident[6]);
        ei_osabi = static_cast<uint8_t>(elfident[7]);
        ei_abiversion = static_cast<uint8_t>(elfident[8]);

        // Read remaining header (big endian)
        char buffer[36]; // 2H5L6H = 2*2 + 5*4 + 6*2 = 4+20+12 = 36 bytes
        fh.read(buffer, 36);
        size_t offset = 0;
        
        type = (static_cast<uint8_t>(buffer[0]) << 8) | static_cast<uint8_t>(buffer[1]);
        offset += 2;
        machine = (static_cast<uint8_t>(buffer[2]) << 8) | static_cast<uint8_t>(buffer[3]);
        offset += 2;
        
        auto read32 = [&](size_t& off) -> uint32_t {
            uint32_t val = (static_cast<uint8_t>(buffer[off]) << 24) |
                          (static_cast<uint8_t>(buffer[off+1]) << 16) |
                          (static_cast<uint8_t>(buffer[off+2]) << 8) |
                          static_cast<uint8_t>(buffer[off+3]);
            off += 4;
            return val;
        };
        
        version = read32(offset);
        entry = read32(offset);
        phoff = read32(offset);
        shoff = read32(offset);
        flags = read32(offset);
        
        auto read16 = [&](size_t& off) -> uint16_t {
            uint16_t val = (static_cast<uint8_t>(buffer[off]) << 8) |
                          static_cast<uint8_t>(buffer[off+1]);
            off += 2;
            return val;
        };
        
        ehsize = read16(offset);
        phentsize = read16(offset);
        phnum = read16(offset);
        shentsize = read16(offset);
        shnum = read16(offset);
        shstrndx = read16(offset);
    }
};

struct PHeader {
    uint32_t type;
    uint32_t offset;
    uint32_t vaddr;
    uint32_t paddr;
    uint32_t filesz;
    uint32_t memsz;
    uint32_t flags;
    uint32_t align;

    PHeader(const EHeader& eh, std::ifstream& fh) {
        std::vector<char> data(eh.phentsize);
        fh.read(data.data(), eh.phentsize);
        
        size_t off = 0;
        auto read32 = [&](size_t& o) -> uint32_t {
            uint32_t val = (static_cast<uint8_t>(data[o]) << 24) |
                          (static_cast<uint8_t>(data[o+1]) << 16) |
                          (static_cast<uint8_t>(data[o+2]) << 8) |
                          static_cast<uint8_t>(data[o+3]);
            o += 4;
            return val;
        };
        
        type = read32(off);
        offset = read32(off);
        vaddr = read32(off);
        paddr = read32(off);
        filesz = read32(off);
        memsz = read32(off);
        flags = read32(off);
        align = read32(off);
    }
};

struct SHeader {
    uint32_t nameidx;
    uint32_t type;
    uint32_t flags;
    uint32_t addr;
    uint32_t offset;
    uint32_t size;
    uint32_t link;
    uint32_t info;
    uint32_t addralign;
    uint32_t entsize;
    std::string name;
    int32_t relidx;

    SHeader(const EHeader& eh, std::ifstream& fh) : relidx(-1) {
        std::vector<char> data(eh.shentsize);
        fh.read(data.data(), eh.shentsize);
        
        size_t off = 0;
        auto read32 = [&](size_t& o) -> uint32_t {
            uint32_t val = (static_cast<uint8_t>(data[o]) << 24) |
                          (static_cast<uint8_t>(data[o+1]) << 16) |
                          (static_cast<uint8_t>(data[o+2]) << 8) |
                          static_cast<uint8_t>(data[o+3]);
            o += 4;
            return val;
        };
        
        nameidx = read32(off);
        type = read32(off);
        flags = read32(off);
        addr = read32(off);
        offset = read32(off);
        size = read32(off);
        link = read32(off);
        info = read32(off);
        addralign = read32(off);
        entsize = read32(off);
    }
};

struct Rela {
    uint32_t offset;
    uint32_t info;
    int32_t addend;
    uint32_t sym;
    uint8_t type;

    Rela(const EHeader& eh, std::ifstream& fh) {
        const size_t relSize = 12; // 2L + l = 8 + 4 = 12
        char data[12];
        fh.read(data, relSize);
        
        offset = (static_cast<uint8_t>(data[0]) << 24) |
                 (static_cast<uint8_t>(data[1]) << 16) |
                 (static_cast<uint8_t>(data[2]) << 8) |
                 static_cast<uint8_t>(data[3]);
        info = (static_cast<uint8_t>(data[4]) << 24) |
               (static_cast<uint8_t>(data[5]) << 16) |
               (static_cast<uint8_t>(data[6]) << 8) |
               static_cast<uint8_t>(data[7]);
        addend = (static_cast<uint8_t>(data[8]) << 24) |
                 (static_cast<uint8_t>(data[9]) << 16) |
                 (static_cast<uint8_t>(data[10]) << 8) |
                 static_cast<uint8_t>(data[11]);
        
        sym = info >> 8;
        type = info & 0xff;
    }
};

struct Symbol {
    uint32_t nameidx;
    uint32_t value;
    uint32_t size;
    uint8_t info;
    uint8_t other;
    uint16_t shndx;
    uint8_t bind;
    uint8_t type;
    std::string name;

    Symbol(const EHeader& eh, std::ifstream& fh) {
        const size_t symSize = 16; // 3L + 2b + H = 12 + 2 + 2 = 16
        char data[16];
        fh.read(data, symSize);
        
        nameidx = (static_cast<uint8_t>(data[0]) << 24) |
                  (static_cast<uint8_t>(data[1]) << 16) |
                  (static_cast<uint8_t>(data[2]) << 8) |
                  static_cast<uint8_t>(data[3]);
        value = (static_cast<uint8_t>(data[4]) << 24) |
                (static_cast<uint8_t>(data[5]) << 16) |
                (static_cast<uint8_t>(data[6]) << 8) |
                static_cast<uint8_t>(data[7]);
        size = (static_cast<uint8_t>(data[8]) << 24) |
               (static_cast<uint8_t>(data[9]) << 16) |
               (static_cast<uint8_t>(data[10]) << 8) |
               static_cast<uint8_t>(data[11]);
        info = static_cast<uint8_t>(data[12]);
        other = static_cast<uint8_t>(data[13]);
        shndx = (static_cast<uint8_t>(data[14]) << 8) | static_cast<uint8_t>(data[15]);
        
        bind = info >> 4;
        type = info & 0xf;
    }
};

class X68kHeader {
public:
    uint32_t base;
    uint32_t entry;
    uint32_t textsz;
    uint32_t datasz;
    uint32_t bsssz;
    uint32_t relsz;
    uint32_t symsz;

    X68kHeader(uint32_t base, uint32_t entry, uint32_t textsz, uint32_t datasz, 
               uint32_t bsssz, uint32_t relsz, uint32_t symsz)
        : base(base), entry(entry), textsz(textsz), datasz(datasz), 
          bsssz(bsssz), relsz(relsz), symsz(symsz) {}

    std::vector<uint8_t> encode() const {
        std::vector<uint8_t> result;
        result.push_back('H');
        result.push_back('U');
        result.push_back(0);
        result.push_back(0);
        
        auto push32 = [&](uint32_t val) {
            result.push_back((val >> 24) & 0xFF);
            result.push_back((val >> 16) & 0xFF);
            result.push_back((val >> 8) & 0xFF);
            result.push_back(val & 0xFF);
        };
        
        push32(base);
        push32(entry);
        push32(textsz);
        push32(datasz);
        push32(bsssz);
        push32(relsz);
        push32(symsz);
        
        for (int i = 0; i < 32; i++) {
            result.push_back(0);
        }
        
        return result;
    }
};

class X68kSymbol {
public:
    uint16_t type;
    uint32_t value;
    std::string name;

    X68kSymbol(uint16_t type, uint32_t value, const std::string& name)
        : type(type), value(value), name(name) {}

    std::vector<uint8_t> encode(uint32_t base = 0) const {
        std::vector<uint8_t> result;
        
        // type (2 bytes, big endian)
        result.push_back((type >> 8) & 0xFF);
        result.push_back(type & 0xFF);
        
        // value (4 bytes, big endian)
        uint32_t val = value + base;
        result.push_back((val >> 24) & 0xFF);
        result.push_back((val >> 16) & 0xFF);
        result.push_back((val >> 8) & 0xFF);
        result.push_back(val & 0xFF);
        
        // name (CP932/SJIS encoded - using raw bytes)
        for (char c : name) {
            result.push_back(static_cast<uint8_t>(c));
        }
        result.push_back(0);
        
        // padding to even length
        size_t nameLen = name.length() + 1;
        if (nameLen & 1 != 0) {
            result.push_back(0);
        }
        
        return result;
    }
};

std::vector<uint8_t> elf2x68k(std::ifstream& fh, uint32_t xbase = 0, bool strip = false) {
    // Read ELF header
    EHeader eh(fh);
    
    // Read program headers
    std::vector<PHeader> phlist;
    fh.seekg(eh.phoff);
    for (int i = 0; i < eh.phnum; i++) {
        phlist.push_back(PHeader(eh, fh));
    }
    
    // Read section headers
    std::vector<SHeader> shlist;
    SHeader* sh_symtab = nullptr;
    std::vector<SHeader> sh_rela;
    int real_rela = 0;
    
    fh.seekg(eh.shoff);
    for (int i = 0; i < eh.shnum; i++) {
        shlist.push_back(SHeader(eh, fh));
        if (shlist.back().type == 2) {
            sh_symtab = &shlist.back();
        } else if (shlist.back().type == 4) {
            real_rela++;
            sh_rela.push_back(shlist.back());
        }
    }
    
    // Read relocations
    std::vector<Rela> rellist;
    for (int i = 0; i < real_rela; i++) {
        if (shlist[sh_rela[i].info].flags & 2) {
            fh.seekg(sh_rela[i].offset);
            while (static_cast<uint32_t>(fh.tellg()) - sh_rela[i].offset < sh_rela[i].size) {
                rellist.push_back(Rela(eh, fh));
            }
        }
    }
    
    // Read symbols
    std::vector<Symbol> symlist;
    if (sh_symtab) {
        fh.seekg(sh_symtab->offset);
        while (static_cast<uint32_t>(fh.tellg()) - sh_symtab->offset < sh_symtab->size) {
            symlist.push_back(Symbol(eh, fh));
        }
        
        SHeader& sh_strtab = shlist[sh_symtab->link];
        fh.seekg(sh_strtab.offset);
        std::vector<char> strtab(sh_strtab.size);
        fh.read(strtab.data(), sh_strtab.size);
        
        for (auto& sym : symlist) {
            size_t idx = sym.nameidx;
            std::string name;
            while (idx < strtab.size() && strtab[idx] != '\0') {
                name += strtab[idx];
                idx++;
            }
            sym.name = name;
        }
    }
    
    // Build sections
    int32_t baseaddr = -1;
    uint32_t curaddr = 0;
    int prevtype = -1;
    std::vector<std::vector<uint8_t>> contents(3);
    
    for (auto& sh : shlist) {
        if (sh.flags & 2) {
            if (baseaddr < 0) {
                baseaddr = sh.addr;
                curaddr = sh.addr;
            }
            if (prevtype >= 0) {
                size_t padding = sh.addr - curaddr;
                for (size_t i = 0; i < padding; i++) {
                    contents[prevtype].push_back(0);
                }
            }
            
            if (sh.type == 1) {
                fh.seekg(sh.offset);
                std::vector<char> data(sh.size);
                fh.read(data.data(), sh.size);
                
                if (!(sh.flags & 1)) {
                    prevtype = 0;
                } else {
                    prevtype = 1;
                }
                contents[prevtype].insert(contents[prevtype].end(), data.begin(), data.end());
            } else if (sh.type == 8) {
                prevtype = 2;
                for (uint32_t i = 0; i < sh.size; i++) {
                    contents[prevtype].push_back(0);
                }
            }
            curaddr = sh.addr + sh.size;
        }
    }
    
    // Combine text and data
    std::vector<uint8_t> body;
    body.insert(body.end(), contents[0].begin(), contents[0].end());
    body.insert(body.end(), contents[1].begin(), contents[1].end());
    
    // Process relocations
    std::vector<uint8_t> reldata;
    std::vector<uint8_t> oreldata;
    uint32_t prevoffset = 0;
    uint32_t oprevoffset = 0;
    
    for (auto& r : rellist) {
        if (symlist[r.sym].shndx != 0 && symlist[r.sym].shndx != 0xfff1 && r.type < 4) {
            uint32_t off = r.offset - baseaddr;

            // Read 4 bytes big endian
            uint32_t val = (body[off] << 24) | (body[off+1] << 16) | (body[off+2] << 8) | body[off+3];
            val = val - baseaddr + xbase;
            
            // Write back 4 bytes big endian
            body[off] = (val >> 24) & 0xFF;
            body[off+1] = (val >> 16) & 0xFF;
            body[off+2] = (val >> 8) & 0xFF;
            body[off+3] = val & 0xFF;
            
            if ((r.offset & 1) == 0) {
                uint32_t offdiff = off - prevoffset;
                prevoffset = off;
                if (offdiff < 0x10000) {
                    reldata.push_back((offdiff >> 8) & 0xFF);
                    reldata.push_back(offdiff & 0xFF);
                } else {
                    reldata.push_back(0);
                    reldata.push_back(1);
                    reldata.push_back((offdiff >> 24) & 0xFF);
                    reldata.push_back((offdiff >> 16) & 0xFF);
                    reldata.push_back((offdiff >> 8) & 0xFF);
                    reldata.push_back(offdiff & 0xFF);
                }
            } else {
                uint32_t offdiff = off - oprevoffset;
                oprevoffset = off;
                if (offdiff < 0x10000) {
                    oreldata.push_back((offdiff >> 8) & 0xFF);
                    oreldata.push_back(offdiff & 0xFF);
                } else {
                    oreldata.push_back(0);
                    oreldata.push_back(1);
                    oreldata.push_back((offdiff >> 24) & 0xFF);
                    oreldata.push_back((offdiff >> 16) & 0xFF);
                    oreldata.push_back((offdiff >> 8) & 0xFF);
                    oreldata.push_back(offdiff & 0xFF);
                }
            }
        }
    }
    
    // Handle odd relocation data
    size_t orlen = oreldata.size();
    if (orlen > 0) {
        for (auto& sym : symlist) {
            if (sym.name == "__cxx_x68k_odd_relocation") {
                uint32_t off = sym.value - baseaddr;
                uint32_t bodyLen = body.size();
                body[off] = (bodyLen >> 24) & 0xFF;
                body[off+1] = (bodyLen >> 16) & 0xFF;
                body[off+2] = (bodyLen >> 8) & 0xFF;
                body[off+3] = bodyLen & 0xFF;
                
                oreldata.push_back(0);
                oreldata.push_back(0);
                
                if (contents[2].size() > orlen) {
                    contents[2].resize(contents[2].size() - orlen);
                } else {
                    contents[2].clear();
                }
                contents[1].insert(contents[1].end(), oreldata.begin(), oreldata.end());
                body.insert(body.end(), oreldata.begin(), oreldata.end());
                break;
            }
        }
    }
    
    // Build symbol table
    std::vector<uint8_t> symtbl;
    if (!strip) {
        for (auto& sym : symlist) {
            if (sym.bind != 1) {
                continue;
            }
            
            SHeader* sh = (sym.shndx < 0xff00) ? &shlist[sym.shndx] : nullptr;
            if (sh && (sh->flags & 2)) {
                uint16_t symtype = 0;
                if (sh->type == 1) {
                    if (!(sh->flags & 1)) {
                        symtype = 0x0201;
                    } else {
                        symtype = 0x0202;
                    }
                } else if (sh->type == 8) {
                    symtype = 0x0203;
                }
                
                if (symtype) {
                    X68kSymbol xsym(symtype, sym.value - baseaddr + xbase, sym.name);
                    auto encoded = xsym.encode();
                    symtbl.insert(symtbl.end(), encoded.begin(), encoded.end());
                }
            }
        }
    }
    
    // Build final output
    X68kHeader xheader(xbase, eh.entry - baseaddr + xbase, 
                       contents[0].size(), contents[1].size(), 
                       contents[2].size(), reldata.size(), symtbl.size());
    std::vector<uint8_t> result = xheader.encode();
    result.insert(result.end(), body.begin(), body.end());
    result.insert(result.end(), reldata.begin(), reldata.end());
    result.insert(result.end(), symtbl.begin(), symtbl.end());
    
    return result;
}

// Simple argument parser class
class ArgumentParser {
private:
    std::string description;
    struct Argument {
        std::string name;
        std::string help;
        std::string shortFlag;
        std::string longFlag;
        bool hasValue;
        bool isFlag;
        std::string defaultValue;
    };
    std::vector<Argument> arguments;
    std::vector<std::string> positionalArgs;

public:
    ArgumentParser(const std::string& desc) : description(desc) {}

    void addArgument(const std::string& name, const std::string& help, 
                     const std::string& shortFlag = "", const std::string& longFlag = "",
                     bool hasValue = false, bool isFlag = false, 
                     const std::string& defaultValue = "") {
        arguments.push_back({name, help, shortFlag, longFlag, hasValue, isFlag, defaultValue});
    }

    void parseArgs(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            bool found = false;
            
            for (auto& argument : arguments) {
                if (arg == argument.shortFlag || arg == argument.longFlag) {
                    if (argument.isFlag) {
                        // Store flag as positional argument with special marker
                        positionalArgs.push_back("__FLAG__" + argument.name);
                    } else if (argument.hasValue && i + 1 < argc) {
                        positionalArgs.push_back("__VALUE__" + argument.name + "=" + argv[++i]);
                    }
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                positionalArgs.push_back(arg);
            }
        }
    }

    std::string getValue(const std::string& name) {
        std::string prefix = "__VALUE__" + name + "=";
        for (const auto& arg : positionalArgs) {
            if (arg.substr(0, prefix.length()) == prefix) {
                return arg.substr(prefix.length());
            }
        }
        return "";
    }

    bool getFlag(const std::string& name) {
        std::string flag = "__FLAG__" + name;
        for (const auto& arg : positionalArgs) {
            if (arg == flag) {
                return true;
            }
        }
        return false;
    }

    std::string getPositional(int index) {
        int pos = 0;
        for (const auto& arg : positionalArgs) {
            if (arg.substr(0, 8) != "__FLAG__" && arg.substr(0, 8) != "__VALUE__") {
                if (pos == index) {
                    return arg;
                }
                pos++;
            }
        }
        return "";
    }
};

int main(int argc, char* argv[]) {
    ArgumentParser parser("ELF to X68k executable converter");
    parser.addArgument("file", "Input ELF file", "", "", false, false);
    parser.addArgument("output", "Output X68k exec file", "-o", "--output", true, false);
    parser.addArgument("base", "Set base address", "-b", "--base", true, false);
    parser.addArgument("strip", "Strip symbol table", "-s", "--strip", false, true);
    
    parser.parseArgs(argc, argv);
    
    std::string inputFile = parser.getPositional(0);
    std::string outputFile = parser.getValue("output");
    std::string baseStr = parser.getValue("base");
    bool strip = parser.getFlag("strip");
    
    uint32_t base = 0;
    if (!baseStr.empty()) {
        // Handle hex (0x prefix) and decimal input
        if (baseStr.length() > 2 && baseStr[0] == '0' && (baseStr[1] == 'x' || baseStr[1] == 'X')) {
            base = std::stoul(baseStr.substr(2), nullptr, 16);
        } else {
            base = std::stoul(baseStr, nullptr, 0);
        }
    }
    
    if (outputFile.empty()) {
        outputFile = inputFile + ".X";
    }
    
    // Open input file
    std::ifstream fi(inputFile, std::ios::binary);
    if (!fi.is_open()) {
        std::cerr << "Error: Could not open input file: " << inputFile << std::endl;
        return 1;
    }
    
    // Open output file
    std::ofstream fo(outputFile, std::ios::binary);
    if (!fo.is_open()) {
        std::cerr << "Error: Could not open output file: " << outputFile << std::endl;
        fi.close();
        return 1;
    }
    
    // Convert ELF to X68k
    std::vector<uint8_t> outputData = elf2x68k(fi, base, strip);
    
    // Write output data
    fo.write(reinterpret_cast<const char*>(outputData.data()), outputData.size());
    
    // Close files
    fi.close();
    fo.close();
    
    return 0;
}

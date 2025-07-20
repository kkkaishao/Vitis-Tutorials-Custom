VCC = v++
VPP_SPEC = system.cfg

# default target is hw_emu
TARGET = hw_emu

# dir definitions
BUILD_DIR = build
AIE_DIR = $(BUILD_DIR)/aie
SW_DIR = $(BUILD_DIR)/sw
PKG_DIR = $(BUILD_DIR)/package_$(TARGET)

# file path definitions
LIBADF = $(AIE_DIR)/libadf.a
PLATFORM = $(PLATFORM_REPO_PATH)/vd100-ext-platform.xpfm
AIE_GRAPH = aie/graph.cpp
HOST_OBJ = $(SW_DIR)/host.o $(SW_DIR)/aie_control_xrt.o
HOST_EXE = $(SW_DIR)/host.exe
XSA = $(BUILD_DIR)/aie_graph_$(TARGET).xsa

# host application compiler flags
CXXFLAGS += -std=c++17 -I${XILINX_VIVADO}/include -I${XILINX_VITIS}/aietools/include -I${SDKTARGETSYSROOT}/usr/include/xrt -I$(AIE_DIR)/Work/ps/c_rts -O0 -g -Wall -fmessage-length=0 --sysroot=${SDKTARGETSYSROOT}
LDFLAGS += -ladf_api_xrt -lgcc -lc -lxrt_coreutil -lxilinxopencl -lpthread -lrt -ldl -lcrypt -lstdc++ -L${SDKTARGETSYSROOT}/usr/lib/ --sysroot=${SDKTARGETSYSROOT} -L$(XILINX_VITIS)/aietools/lib/aarch64.o

# environment variables check
check_env = \
	$(strip $(foreach 1,$1, \
		$(call __check_env,$1,$(strip $(value 2)))))

__check_env = \
	$(if $(value $1),, \
		$(error Undefined $1$(if $2, ($2))))

all: aie xsa host package

check_PLATFORM_REPO_PATH:
	$(call check_env, PLATFORM_REPO_PATH, Please set the PLATFORM_REPO_PATH environment variable to the path of your custom Vitis platform repository.)

check_ROOTFS:
	$(call check_env, ROOTFS, Set to: xilinx-versal-common-v2025.1/rootfs.ext4)

check_IMAGE:
	$(call check_env, IMAGE, Set to: xilinx-versal-common-v2025.1/Image)

check_SDKTARGETSYSROOT:
	$(call check_env, SDKTARGETSYSROOT, Source the script {PETALINUX_INSTALL_DIR}/environment-setup-cortexa72-cortexa53-amd-linux.)

check_CXX:
	$(call check_env, CXX, Please source the script {PETALINUX_INSTALL_DIR}/environment-setup-cortexa72-cortexa53-amd-linux.)

.PHONY: all clean package test check_env aie aiesim host xsa

$(AIE_DIR): 
	mkdir -p $(AIE_DIR)

$(SW_DIR):
	mkdir -p $(SW_DIR)

$(PKG_DIR):
	mkdir -p $(PKG_DIR)

aie: check_PLATFORM_REPO_PATH $(AIE_GRAPH) $(AIE_DIR) $(LIBADF)
$(LIBADF): aie/*
	cd $(AIE_DIR) && \
	$(VCC) -c --mode aie \
		--platform $(PLATFORM) \
		--include $(PWD)/aie \
		--work_dir=Work $(PWD)/$(AIE_GRAPH) 2>&1 | tee log.txt && \
	echo "Copying Work/ps/c_rts/aie_control_xrt.cpp to sw/aie_control_xrt.cpp" && \
	cp Work/ps/c_rts/aie_control_xrt.cpp $(PWD)/sw/aie_control_xrt.cpp

aiesim: $(LIBADF)
	cd $(AIE_DIR) && \
	aiesimulator --pkg-dir=Work --dump-vcd sim

$(SW_DIR)/%.o: sw/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(HOST_EXE): $(HOST_OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

host: check_CXX check_SDKTARGETSYSROOT $(SW_DIR) $(HOST_EXE)

xsa: check_PLATFORM_REPO_PATH $(XSA)
$(XSA): $(LIBADF) $(VPP_SPEC)
	cd $(BUILD_DIR) && \
	$(VCC) -g -l \
		-t $(TARGET) \
		--platform $(PLATFORM) \
		--save-temps --verbose \
		--config $(PWD)/$(VPP_SPEC) \
		-o $(PWD)/$(XSA) \
		$(PWD)/$(LIBADF)

package: check_PLATFORM_REPO_PATH check_ROOTFS check_IMAGE $(PKG_DIR) package_$(TARGET)
package_$(TARGET): $(LIBADF) $(XSA) $(HOST_EXE)
	cd $(BUILD_DIR) && \
	$(VCC) -p -t $(TARGET) \
		--platform $(PLATFORM) \
		--package.rootfs ${ROOTFS} \
		--package.kernel_image ${IMAGE} \
		--package.boot_mode=sd \
		--package.image_format=ext4 \
		--package.defer_aie_run \
		--package.sd_file $(PWD)/embedded_exec.sh \
		--package.sd_file $(PWD)/$(HOST_EXE) \
		--package.out_dir package_$(TARGET) \
		$(PWD)/${XSA} $(PWD)/${LIBADF}

emu:
	./$(BUILD_DIR)/package_hw_emu/launch_hw_emu.sh

clean:
	cd $(BUILD_DIR) && \
	rm -rf .Xil _x *.log *.jou *.xsa *.xclbin* v++_* *_summary
	cd $(AIE_DIR) && \
	rm -rf .Xil Work *.log libadf.a log.txt sol.db Map_Report.csv *.vcd aiesimu* x86simu*
	cd $(SW_DIR) && \
	rm -rf *.o *.o.* *.exe





import vitis
import os

pwd = os.path.dirname(os.path.realpath(__file__))

client = vitis.create_client()
client.set_workspace(path="./workspace")

plat_name = "alinx-vd100"

# Create a new platform with AIE domain
plat_obj = client.create_platform_component(
	name=plat_name,
	hw_design="../export/platform.xsa",
	emu_design="../export/platform-emu.xsa",
	desc="Alinx VD100 Vitis Extensible Platform",
	os="aie_runtime",
	cpu="ai_engine",
	domain_name="aie",
)
# Add Linux domain to the platform
# Turn off DTB generation since we have to use a customized device tree file.
plat_obj.add_domain(
	name="xrt",
	display_name="xrt",
	os="linux",
	cpu="psv_cortexa72",
	generate_dtb=False,
)

# Generate bif file for linux domain 
xrt = plat_obj.get_domain("xrt")
xrt.generate_bif()

# Set the pre-built image directory
# get PATH from env
image_dir = os.getenv("IMAGE_DIR")
if not image_dir:
    raise ValueError("IMAGE_DIR environment variable is not set. Please set to the directory containing petalinux image files.")
xrt.set_boot_dir(image_dir)

# Manually set the dtb path to use customized device tree file.
# Since we need to modify sdhci node as a workaround for the SD card issue.
xrt.set_dtb("../../petalinux/system.dtb")

plat_obj.build()

vitis.dispose()
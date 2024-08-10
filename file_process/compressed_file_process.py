import os
import zipfile
import tarfile
from pathlib import Path

class CompressedFileExtractor:
	def __init__(self, base_dir):
		self.base_dir = Path(base_dir)
		self.processed = set()
		
	def extract_all(self, directory=None):
		if directory is None:
			directory = self.base_dir
			
		for item in directory.iterdir():
			if item.is_file() and self.is_archive(item) and str(item) not in self.processed:
				print(f"解压: {item.name}")
				self.extract_archive(item)
				self.processed.add(str(item))
				
				# 处理刚解压的目录
				output_dir = item.parent / f"{item.stem}_extracted"
				if output_dir.exists():
					self.extract_all(output_dir)
					
			elif item.is_dir():
				self.extract_all(item)
	
	def is_archive(self, file_path):
		return file_path.suffix.lower() in {'.zip', '.tar'}
	
	def extract_archive(self, file_path):
		output_dir = file_path.parent / f"{file_path.stem}_extracted"
		output_dir.mkdir(exist_ok=True)
		
		try:
			if file_path.suffix.lower() == '.zip':
				with zipfile.ZipFile(file_path, 'r') as zip_ref:
					zip_ref.extractall(output_dir)
			elif file_path.suffix.lower() == '.tar':
				with tarfile.open(file_path, 'r') as tar_ref:
					tar_ref.extractall(output_dir)
			print(f"  成功解压到: {output_dir}")
		except Exception as e:
			print(f"  解压失败: {e}")

if __name__ == "__main__":
	extractor = CompressedFileExtractor(".")  # 当前目录
	extractor.extract_all()
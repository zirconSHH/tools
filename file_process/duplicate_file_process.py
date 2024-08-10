import os
import re
from pathlib import Path
from collections import defaultdict

class DuplicateFileCleaner:
	def __init__(self, base_dir):
		self.base_dir = Path(base_dir)
		self.deleted_files = []
		self.promoted_files = []  # 记录被提升为原文件的文件
	
	def extract_base_name(self, filename):
		"""提取基础文件名，去除所有括号和数字"""
		# 移除所有括号和数字
		base = re.sub(r'\(\d+\)', '', filename)
		return base.strip()
	
	def has_parentheses(self, filename):
		"""检查文件名是否包含括号"""
		return '(' in filename and ')' in filename
	
	def get_original_filename(self, base_name):
		"""根据基础名称生成原始文件名"""
		return base_name
	
	def find_best_candidate(self, file_list):
		"""从文件列表中找出最佳候选作为原文件"""
		if not file_list:
			return None
			
		# 首先尝试找到没有括号的文件
		no_parentheses_files = [f for f in file_list if not self.has_parentheses(f.name)]
		if no_parentheses_files:
			return no_parentheses_files[0]
		
		# 如果没有不带括号的文件，选择括号数字最小的文件
		def get_parentheses_number(file_path):
			name = file_path.name
			# 提取第一个括号内的数字
			match = re.search(r'\((\d+)\)', name)
			return int(match.group(1)) if match else float('inf')
		
		# 按括号数字排序，数字小的在前
		sorted_files = sorted(file_list, key=get_parentheses_number)
		return sorted_files[0]
	
	def rename_file_to_original(self, file_path, original_name):
		"""将文件重命名为原始文件名"""
		try:
			original_path = file_path.parent / original_name
			
			# 如果目标文件名已存在，先删除它（这种情况应该很少见）
			if original_path.exists():
				original_path.unlink()
				
			file_path.rename(original_path)
			return original_path
		except Exception as e:
			print(f"  重命名失败 {file_path.name} -> {original_name}: {e}")
			return file_path
	
	def clean_directory(self, directory=None):
		"""递归清理重复文件"""
		if directory is None:
			directory = self.base_dir
		
		files_by_base = defaultdict(list)
		
		# 收集当前目录的文件
		for item in directory.iterdir():
			if item.is_file():
				base_name = self.extract_base_name(item.name)
				files_by_base[base_name].append(item)
			elif item.is_dir():
				self.clean_directory(item)
		
		# 处理重复文件
		for base_name, file_list in files_by_base.items():
			self.process_duplicate_group(base_name, file_list)
	
	def process_duplicate_group(self, base_name, file_list):
		"""处理一组重复文件"""
		original_filename = self.get_original_filename(base_name)
		
		# 检查原文件是否存在
		original_exists = any(f.name == original_filename for f in file_list)
		
		if original_exists:
			# 原文件存在，删除所有带括号的重复文件
			self.delete_duplicates_with_original(original_filename, file_list)
		else:
			# 原文件不存在，将最佳候选提升为原文件，删除其他重复文件
			self.promote_and_delete_duplicates(base_name, file_list)
	
	def delete_duplicates_with_original(self, original_filename, file_list):
		"""当原文件存在时，删除所有带括号的重复文件"""
		# print(f"原文件存在: {original_filename}")
		
		for file_path in file_list:
			if file_path.name != original_filename and self.has_parentheses(file_path.name):
				try:
					file_size = file_path.stat().st_size
					file_path.unlink()
					self.deleted_files.append(str(file_path))
					print(f"  已删除: {file_path.name} (大小: {file_size} 字节)")
				except Exception as e:
					print(f"  删除失败 {file_path.name}: {e}")
	
	def promote_and_delete_duplicates(self, base_name, file_list):
		"""当原文件不存在时，将最佳候选提升为原文件，删除其他重复文件"""
		original_filename = self.get_original_filename(base_name)
		best_candidate = self.find_best_candidate(file_list)
		
		if not best_candidate:
			print(f"警告: 无法确定最佳候选文件，基名: {base_name}")
			return
		
		print(f"原文件不存在，提升最佳候选: {best_candidate.name} -> {original_filename}")
		
		# 重命名最佳候选为原文件名
		new_path = self.rename_file_to_original(best_candidate, original_filename)
		if new_path != best_candidate:
			self.promoted_files.append(f"{best_candidate.name} -> {original_filename}")
			print(f"  已重命名: {best_candidate.name} -> {original_filename}")
		
		# 删除其他重复文件
		for file_path in file_list:
			if file_path != best_candidate and file_path.exists():
				try:
					file_size = file_path.stat().st_size
					file_path.unlink()
					self.deleted_files.append(str(file_path))
					print(f"  已删除: {file_path.name} (大小: {file_size} 字节)")
				except Exception as e:
					print(f"  删除失败 {file_path.name}: {e}")
	
	def clean_all(self):
		"""开始清理"""
		print(f"开始清理目录: {self.base_dir}")
		print("=" * 50)
		
		self.clean_directory()
		
		print("=" * 50)
		print("清理完成！")
		
		if self.promoted_files:
			print(f"\n总共提升了 {len(self.promoted_files)} 个文件:")
			for promoted in self.promoted_files:
				print(f"  - {promoted}")
		
		if self.deleted_files:
			print(f"\n总共删除了 {len(self.deleted_files)} 个重复文件:")
			for deleted in self.deleted_files[:10]:  # 只显示前10个
				print(f"  - {deleted}")
			if len(self.deleted_files) > 10:
				print(f"  ... 和 {len(self.deleted_files) - 10} 个其他文件")
		else:
			print("\n没有找到需要删除的重复文件")

# 使用示例
if __name__ == "__main__":
	cleaner = DuplicateFileCleaner(r"D:\视频\porn\QQ")  # 当前目录
	cleaner.clean_all()
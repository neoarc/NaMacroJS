  text = File.read('version.h.template')
  commitCount = `git rev-list --count --first-parent HEAD`.chop
  newContent = text.gsub(/\[COMMIT_COUNT\]/, commitCount)
  File.open('version.h', "w") { |file| file.puts newContent }
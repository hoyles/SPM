#' Helper function for 'extract'
#'
#' @author Alistair Dunn
#' @param lines list of lines to process
#'
"extract.agesize" <- function(lines){
  if(missing(lines)) stop("ERROR: Missing argument lines")
  index.start<-(1:length(lines))[substring(lines,1,1)=="["][1]
  index.end<-(1:length(lines))[substring(lines,1,4)=="*end"][1]
  if(index.start >= index.end) stop("Error")
  res<-list()
  res$report.type<-substring(lines[index.start+1],14)
  res$label<-substring(lines[index.start+2],8)
  ages<-spm.string.to.vector.of.numbers(substring(lines[index.start+3],regexpr(":",lines[index.start+3])+1))
  sizes<-spm.string.to.vector.of.numbers(substring(lines[index.start+4],regexpr(":",lines[index.start+4])+1))
  weights<-spm.string.to.vector.of.numbers(substring(lines[index.start+5],regexpr(":",lines[index.start+5])+1))
  res$data<-list("ages"=ages,"sizes"=sizes,"weights"=weights)
  return(res)
}
